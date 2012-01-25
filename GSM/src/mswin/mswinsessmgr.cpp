#include <Windows.h>
#include <set>
#include <cassert>
#include "../../isessionmgr.hpp"
#include "../../compmgr.hpp"
#include "../../util/format.hpp"
#include "../../idisplay.hpp"
#include "../../igfxres.hpp"
#include "mswincanvas.hpp"
#include "mswinerr.hpp"
#include "mswinsurf.hpp"
#include "mswinevt.hpp"
#include "mswinogl.hpp"
#include "mswinsessmgr.hpp"

namespace gsm {

//--- INTERNAL CONSTANTS ------------------------------------------------------

static const char *WINDOW_CLASS_NAME = "GSM Window Class";

//--- PRIVATE TYPES -----------------------------------------------------------

struct CreateParams {
    MSWinSurface *surface;
    ISurface::Attributes surf_attribs;
    IDisplay *display;
};

//--- PRIVATE FUNCTIONS -------------------------------------------------------

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    MSWinSurface *surf = (MSWinSurface*) GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch(msg)
    {
    case WM_NCCREATE:
        {
            // Re-package user data pointer
            LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            CreateParams *cp = reinterpret_cast<CreateParams*>(pcs->lpCreateParams);
            IDisplay *disp = cp->display;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)cp->surface);
            // Assign window handle to Surface
            cp->surface->setWindowHandle(hWnd);
            // Setup OpenGL if asked for
            if (cp->surf_attribs.test(ISurface::SUPPORTS_OPENGL)) {
                setupWindowForOpenGL(cp->surface, cp->surf_attribs);
            }
        }
        break;
	case WM_CREATE: // TODO (perhaps): OpenGL wiki recommends creating GL context here
		{
			/*char buffer[256];
			GetWindowTextA(hWnd, buffer, sizeof(buffer) );
			OutputDebugString(format("WM_CREATE on window \"%s\"\n", buffer).c_str() );*/
            if (surf->openGLContext() != 0) {
                HDC hDC = GetDC(hWnd);
                OutputDebugString(format("WM_CREATE with hGLRC = %x\n", surf->openGLContext()).c_str() );
            CHECK(wglMakeCurrent, (hDC, surf->openGLContext()));
            }
            surf->display()->onInit();
		}
		break;
	case WM_ACTIVATE:
		{
			/*char buffer[256];
			GetWindowTextA(hWnd, buffer, sizeof(buffer) );
			OutputDebugStringA(format("WM_ACTIVATE on window \"%s\"\n", buffer).c_str() );*/
		}
		break;
	case WM_ERASEBKGND: 
		//OutputDebugString("WM_ERASEBKGND\n");
		return 1;
		break;
	case WM_SIZE:
        if (surf->openGLContext() != 0) {
            HDC hDC = GetDC(hWnd);
            OutputDebugString(format("WM_SIZE with hGLRC = %x\n", surf->openGLContext()).c_str() );
            CHECK(wglMakeCurrent, (hDC, surf->openGLContext()));
        }
        surf->display()->onResize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_CLOSE:
        // The lookup isn't too bad as it only happens once at the end of the window's lifecycle
        surf->display()->onClose();
        static_cast<MSWinSessionManager*>(findComponent("SessionManager"))->closeMsgReceived(hWnd);
		break;
	case WM_PAINT: 
        {
            MSWinCanvas cnv;
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);
                if (surf->openGLContext() != 0) {
                    CHECK(wglMakeCurrent, (hDC, surf->openGLContext()));
                }
                bool done = surf->display()->onPaint(&cnv, surf->videoContextID());
            EndPaint(hWnd, &ps);
            if (done) return 0;
        }
		break;
	case WM_SYSCOMMAND:
		switch (wParam) {    
		case SC_KEYMENU:
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

static void
registerWindowClass()
{
    WNDCLASS wc;
    //wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // | CS_DBLCLKS;	//AB(20.11.07) Added CS_OWNDC
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = NULL;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL; 
    wc.lpszClassName = WINDOW_CLASS_NAME;
    //wc.hIconSm = NULL; 
    if (!RegisterClass(&wc))
        throw EMSWinError(GetLastError(), "RegisterClass");
}

static void
unregisterWindowClass()
{
    // TODO: this can fail because no one takes care of closing windows that were left open

    if (! UnregisterClass(WINDOW_CLASS_NAME, NULL))
        throw EMSWinError(GetLastError(), "UnregisterClass");
}

static void
getScreenRect(int num, int & x, int & y, unsigned & w, unsigned & h)
{
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(dd);
    CHECK(EnumDisplayDevices, (NULL, num, &dd, 0));

    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    CHECK(EnumDisplaySettings, (dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm));

    x = dm.dmPosition.x;
    y = dm.dmPosition.y;
    w = dm.dmPelsWidth;
    h = dm.dmPelsHeight;
}


//--- DISPLAY MANAGER IMPLEMENTATION ------------------------------------------

MSWinSessionManager::MSWinSessionManager()
{
    quit = false;
    have_evt = false;
    registerWindowClass();
}

MSWinSessionManager::~MSWinSessionManager()
{
    unregisterWindowClass();
}

ISurface *
MSWinSessionManager::openWindow(int x, int y, int w, int h, const char *caption, IDisplay *window, ISurface::Attributes attribs)
{
    HWND hWnd;

    MSWinSurface * surf = new MSWinSurface(window);

    CreateParams cp;
    cp.surface = surf;
    cp.surf_attribs = attribs;
    cp.display = window;

    hWnd = CreateWindowEx( 0, WINDOW_CLASS_NAME
        , caption != NULL ? caption : "GSM Window"
        , WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
        , x, y, w, h
        , NULL, NULL, NULL /*_module_instance()*/, &cp
        ); 
    if (hWnd == NULL) throw EMSWinError(GetLastError(), "CreateWindowEx");

    surfaces.insert(surf);

    return surf;
}

ISurface *
MSWinSessionManager::openScreen(int num, ISurface::Attributes attr, IDisplay *screen, ISurface::Attributes attribs)
{
    HWND hWnd;

    if (attribs.test(ISurface::SUPPORTS_OPENGL)) initGlew();

    int x, y;
    unsigned w, h;
    getScreenRect(num, x, y, w, h);

    MSWinSurface * surf = new MSWinSurface(screen);

    CreateParams cp;
    cp.surface = surf;
    cp.surf_attribs = attribs;
    cp.display = dynamic_cast<IDisplay*>(screen);

    hWnd = CreateWindowEx( 0, WINDOW_CLASS_NAME
        , "GSM full-screen window" // TODO: caption != NULL ? caption : wclsname.c_str()
        , WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
        , x, y, w, h // position and size
        , NULL, NULL, NULL /*_module_instance()*/, screen
        ); 
    if (hWnd == NULL) throw EMSWinError(GetLastError(), "CreateWindowEx");

    surfaces.insert(surf);

    return surf;
}

bool
MSWinSessionManager::fetchNextEvent()
{
    assert(!have_evt);

    for (surface_iterator_t it = surfaces.begin(); it != surfaces.end(); it ++) 
    {
        if (PeekMessage(&evt.msg, (*it)->hWnd, 0, 0, PM_REMOVE) ) 
        {
            if (evt.msg.message == WM_QUIT) {
                quit = true;
                return false;
            }
            else {
                have_evt = true;
                if (TranslateMessage(&evt.msg))
                    return true;
                DispatchMessage(&evt.msg);
                return true;
            }
        }
    }
    return false;
}

IEvent *
MSWinSessionManager::getEvent()
{
    if (have_evt) {
        have_evt = false;
        return &evt;
    }
    else
        return NULL;
}

bool
MSWinSessionManager::mustQuit()
{
    return quit || surfaces.empty();
}

MSWinSessionManager::MetaKeys
MSWinSessionManager::getMetaKeyStates()
{
    unsigned long result = 0;

    if ((GetKeyState(VK_SHIFT) & 0x8000) != 0)
        result |= 1 << SHIFT;
    if ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
        result |= 1 << CONTROL;
    if ((GetKeyState(VK_MENU) & 0x8000) != 0)
        result |= 1 << ALT;
    if ((GetKeyState(VK_CAPITAL) & 0x8001) != 0)
        result |= 1 << CAPSLOCK;
    if ((GetKeyState(VK_NUMLOCK) & 0x8001) != 0)
        result |= 1 << NUMLOCK;

    return result;
}

//--- INTERNAL METHODS --------------------------------------------------------

void
MSWinSessionManager::closeMsgReceived(HWND hWnd)
{
    for (surface_iterator_t it = surfaces.begin(); it != surfaces.end(); it++) {
        if ((*it)->hWnd == hWnd) {
            delete (*it);
            surfaces.erase(it);
            break;
        }
    }
}

} // ns gsm
