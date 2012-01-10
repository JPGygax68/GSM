#include <Windows.h>
#include <set>
#include "../../isessionmgr.hpp"
#include "../../compmgr.hpp"
#include "../../util/format.hpp"
#include "../../iwindow.hpp"
#include "mswincanvas.hpp"
#include "mswinerr.hpp"
#include "mswinsurf.hpp"
#include "mswinevt.hpp"
#include "mswinsessmgr.hpp"

namespace gsm {

//--- INTERNAL CONSTANTS ------------------------------------------------------

static const char *WINDOW_CLASS_NAME = "GSM Window Class";

//--- PRIVATE FUNCTIONS -------------------------------------------------------

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    IWindow *win = reinterpret_cast<IWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch(msg)
	{
    case WM_NCCREATE:
        {
            // Re-package user data pointer
            LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            win = reinterpret_cast<IWindow*>(pcs->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win) );
        }
        break;
	case WM_CREATE: // TODO (perhaps): OpenGL wiki recommends creating GL context here
		{
			char buffer[256];
			GetWindowTextA(hWnd, buffer, sizeof(buffer) );
			OutputDebugString(format("WM_CREATE on window \"%s\"\n", buffer).c_str() );
		}
		break;
	case WM_ACTIVATE:
		{
			char buffer[256];
			GetWindowTextA(hWnd, buffer, sizeof(buffer) );
			OutputDebugStringA(format("WM_ACTIVATE on window \"%s\"\n", buffer).c_str() );
		}
		break;
	case WM_ERASEBKGND: 
		//OutputDebugString("WM_ERASEBKGND\n");
		//return 0;
		break;
	case WM_SIZE:
        if (win != NULL) {
            win->onResize(LOWORD(lParam), HIWORD(lParam));
        }
		break;
	case WM_CLOSE:
        // The lookup isn't too bad as it only happens once at the end of the window's lifecycle
        static_cast<MSWinSessionManager*>(findComponent("SessionManager"))->closeMsgReceived(hWnd);
		break;
	case WM_PAINT: 
		if (win != NULL) {
            MSWinCanvas cnv;
            win->onPaint(&cnv);
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
    if (UnregisterClass(WINDOW_CLASS_NAME, NULL) == 0)
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
    registerWindowClass();
}

MSWinSessionManager::~MSWinSessionManager()
{
    unregisterWindowClass();
}

ISurface *
MSWinSessionManager::openWindow(int x, int y, int w, int h, const char *caption, IWindow *window)
{
    HWND hWnd;

    hWnd = CreateWindowEx( 0, WINDOW_CLASS_NAME
        , caption != NULL ? caption : "GSM Window"
        , WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
        , x, y, w, h
        , NULL, NULL, NULL /*_module_instance()*/, window
        ); 
    if (hWnd == NULL) throw EMSWinError(GetLastError(), "CreateWindowEx");

    MSWinSurface * surf = new MSWinSurface(hWnd);
    surfaces.insert(surf);

    return surf;
}

ISurface *
MSWinSessionManager::openScreen(int num, ISurface::Attributes attr, IScreen *screen)
{
    HWND hWnd;

    int x, y;
    unsigned w, h;
    getScreenRect(num, x, y, w, h);

    hWnd = CreateWindowEx( 0, WINDOW_CLASS_NAME
        , "GSM full-screen window" // TODO: caption != NULL ? caption : wclsname.c_str()
        , WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
        , x, y, w, h // position and size
        , NULL, NULL, NULL /*_module_instance()*/, screen
        ); 
    if (hWnd == NULL) throw EMSWinError(GetLastError(), "CreateWindowEx");

    MSWinSurface * surf = new MSWinSurface(hWnd);
    surfaces.insert(surf);

    return surf;
}

bool
MSWinSessionManager::processNextEvent()
{
    for (surface_iterator_t it = surfaces.begin(); it != surfaces.end(); it ++) 
    {
        if (PeekMessage(&msg, (*it)->hWnd, 0, 0, PM_REMOVE) ) 
        {
            if (msg.message == WM_QUIT) {
                quit = true;
            }
            else {
                if (TranslateMessage(&msg))
                    continue;
                DispatchMessage(&msg);
                return true;
            }
        }
    }
    return false;
}

bool
MSWinSessionManager::mustQuit()
{
    return quit || surfaces.empty();
}

//--- INTERNAL METHODS --------------------------------------------------------

void
MSWinSessionManager::closeMsgReceived(HWND hWnd)
{
    for (surface_iterator_t it = surfaces.begin(); it != surfaces.end(); it++) {
        if ((*it)->hWnd == hWnd) {
            surfaces.erase(it);
            break;
        }
    }
}

} // ns gsm
