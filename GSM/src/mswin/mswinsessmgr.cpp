/*-----------------------------------------------------------------------------
 * Graphical Session Manager (GSM)
 *
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <set>
#include <cassert>
#include "../../isessionmgr.hpp"
#include "../../compmgr.hpp"
#include "../../util/format.hpp"
#include "../../idisplay.hpp"
#include "../../iptrmovevt.hpp"
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

struct GetScreenResolution_Data {
    int requested_monitor_num;
    int current_monitor_num;
    int size;
    int pixels;
};

struct CreateParams {
	MSWinSurface* surface = nullptr;
	ISurface::Attributes surf_attribs = 0;
    IDisplay *display = nullptr;
};

class MSWinPointerMoveEvent: public IPointerMotionEvent {
public:
    virtual const Position position() {
        return pos;
    }

private:
    MSWinPointerMoveEvent(LPARAM lParam) {
        pos.x = LOWORD(lParam);
        pos.y = HIWORD(lParam);
    }

    Position pos;

    friend LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

class MSWinPointerButtonEvent: public IPointerButtonEvent {
public:
    enum Direction { DOWN = 1, UP = 2, DOUBLE = 3 };

    virtual const Position position() { return pos; }

    virtual Button button() { return btn; }
    virtual bool pressed() { return dir == DOWN; }
    virtual bool released() { return dir == UP; }
    virtual bool doubleClick() { return dir == DOUBLE; }

private:

    MSWinPointerButtonEvent(LPARAM lParam, Button btn_, Direction dir_) {
        pos.x = LOWORD(lParam);
        pos.y = HIWORD(lParam);
        btn = btn_;
        dir = dir_;
    }

    Position pos;
    Button btn;
    Direction dir;

    friend LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

class MSWinKeyboardEvent: public IKeyboardEvent {
public:
    virtual bool down() { return type == KEY_DOWN; }
    virtual bool up() { return type == KEY_UP; }
    virtual bool isCharacter() { return type == CHARACTER; }
    virtual unicode_t unicode() { return ch; }
    virtual keycode_t keyCode() { return key; }

private:
    enum Type { KEY_DOWN = 1, KEY_UP = 2, CHARACTER = 3 };

    MSWinKeyboardEvent(Type type_, int key_, unicode_t ch_): type(type_), key(key_), ch(ch_) {}

    Type type;
    int key;
    unicode_t ch;

    friend LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

//--- PRIVATE FUNCTIONS -------------------------------------------------------

static IPointerButtonEvent::Button
wParamToXButton(WPARAM wParam)
{
    switch ( HIWORD(wParam) ) {
        case XBUTTON1: return IPointerButtonEvent::EXTENDED_1;
        case XBUTTON2: return IPointerButtonEvent::EXTENDED_2;
        default: return IPointerButtonEvent::NONE;
    }
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    MSWinSurface *surf = reinterpret_cast<MSWinSurface*>( GetWindowLongPtr(hWnd, GWLP_USERDATA) );

    switch(msg)
    {
    case WM_NCCREATE:
        {
            // Re-package user data pointer
            LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            CreateParams *cp = reinterpret_cast<CreateParams*>(pcs->lpCreateParams);
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
                //OutputDebugString(format("WM_CREATE with hGLRC = %x\n", surf->openGLContext()).c_str() );
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
            //OutputDebugString(format("WM_SIZE with hGLRC = %x\n", surf->openGLContext()).c_str() );
            CHECK(wglMakeCurrent, (hDC, surf->openGLContext()));
        }
        surf->display()->onResize(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_CLOSE:
        // The lookup isn't too bad as it only happens once at the end of the window's lifecycle
        if (! surf->display()->onClose()) {
            return 0; // we don't call DefWindowProc, so the window won't be closed
        }
        static_cast<MSWinSessionManager*>(findComponent("SessionManager"))->closeMsgReceived(hWnd);
        break; // Let DefWindowProc destroy the window
    case WM_PAINT: 
        {
            MSWinCanvas cnv;
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);
                if (surf->openGLContext() != 0) {
                    //CHECK(wglMakeCurrent, (hDC, surf->openGLContext()));
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

    //--- MOUSE -------------------------------------------

	case WM_MOUSEMOVE:
        {
            MSWinPointerMoveEvent evt(lParam);
            surf->display()->onPointerMotionEvent( &evt );
        }
        break;
    case WM_LBUTTONDOWN:
        {
            MSWinPointerButtonEvent evt(lParam, IPointerButtonEvent::LEFT, MSWinPointerButtonEvent::DOWN);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;
    case WM_LBUTTONUP:
        {
            MSWinPointerButtonEvent evt(lParam, IPointerButtonEvent::LEFT, MSWinPointerButtonEvent::UP);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;
    case WM_LBUTTONDBLCLK:
        {
            MSWinPointerButtonEvent evt(lParam, IPointerButtonEvent::LEFT, MSWinPointerButtonEvent::DOUBLE);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;
    case WM_RBUTTONDOWN:
        {
            MSWinPointerButtonEvent evt(lParam, IPointerButtonEvent::RIGHT, MSWinPointerButtonEvent::DOWN);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;
    case WM_RBUTTONUP:
        {
            MSWinPointerButtonEvent evt(lParam, IPointerButtonEvent::RIGHT, MSWinPointerButtonEvent::UP);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;
    case WM_RBUTTONDBLCLK:
        {
            MSWinPointerButtonEvent evt(lParam, IPointerButtonEvent::RIGHT, MSWinPointerButtonEvent::DOUBLE);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;
    case WM_MBUTTONDOWN:
        {
            MSWinPointerButtonEvent evt(lParam, IPointerButtonEvent::MIDDLE, MSWinPointerButtonEvent::DOWN);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;
    case WM_MBUTTONUP:
        {
            MSWinPointerButtonEvent evt(lParam, IPointerButtonEvent::MIDDLE, MSWinPointerButtonEvent::UP);
            surf->display()->onPointerButtonEvent( &evt );
        }
    case WM_MBUTTONDBLCLK:
        {
            MSWinPointerButtonEvent evt(lParam, IPointerButtonEvent::MIDDLE, MSWinPointerButtonEvent::DOUBLE);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;
    case WM_XBUTTONDOWN:
        {
            IPointerButtonEvent::Button btn = wParamToXButton(wParam);
            MSWinPointerButtonEvent evt(lParam, btn, MSWinPointerButtonEvent::DOWN);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;
    case WM_XBUTTONUP:
        {
            IPointerButtonEvent::Button btn = wParamToXButton(wParam);
            MSWinPointerButtonEvent evt(lParam, btn, MSWinPointerButtonEvent::UP);
            surf->display()->onPointerButtonEvent( &evt );
        }
    case WM_XBUTTONDBLCLK:
        {
            IPointerButtonEvent::Button btn = wParamToXButton(wParam);
            MSWinPointerButtonEvent evt(lParam, btn, MSWinPointerButtonEvent::DOUBLE);
            surf->display()->onPointerButtonEvent( &evt );
        }
        break;

    //--- KEYBOARD ----------------------------------------

    case WM_KEYDOWN: case WM_SYSKEYDOWN:
        {
            MSWinKeyboardEvent evt(MSWinKeyboardEvent::KEY_DOWN, static_cast<int>(wParam), 0);
            surf->display()->onKeyboardEvent( &evt );
        }
        break;
    case WM_KEYUP: case WM_SYSKEYUP:
        {
            MSWinKeyboardEvent evt(MSWinKeyboardEvent::KEY_UP, static_cast<int>(wParam), 0);
            surf->display()->onKeyboardEvent( &evt );
        }
        break;
    case WM_CHAR:
        {
            MSWinKeyboardEvent evt(MSWinKeyboardEvent::CHARACTER, 0, static_cast<unicode_t>(wParam));
            surf->display()->onKeyboardEvent( &evt );
        }
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

static void
registerWindowClass()
{
    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_OWNDC | CS_DBLCLKS;
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

    if (! UnregisterClass(WINDOW_CLASS_NAME, NULL)) {
        // TODO: log!
        //throw EMSWinError(GetLastError(), "UnregisterClass");
    }
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

static BOOL CALLBACK
MonitorEnumProc(__in  HMONITOR hMonitor, __in  HDC hdcMonitor, __in  LPRECT lprcMonitor, __in  LPARAM dwData)
{
    GetScreenResolution_Data *data = reinterpret_cast<GetScreenResolution_Data*>(dwData);
    
    if (data->current_monitor_num == data->requested_monitor_num) {
        data->size = GetDeviceCaps(hdcMonitor, VERTSIZE);
        data->pixels = GetDeviceCaps(hdcMonitor, VERTRES);
        return FALSE;
    }
    else
        return TRUE;
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

unsigned
MSWinSessionManager::getScreenResolution(int num)
{
    GetScreenResolution_Data data;
    HDC hDC = GetDC(NULL);
    data.current_monitor_num = 0;
    data.pixels = data.size = 0;
    data.requested_monitor_num = num;

    EnumDisplayMonitors(hDC, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&data));
    unsigned result = 0;
    if (data.size > 0) {
        result = data.pixels * 10 * 254 / data.size / 100;
    }
    else
        result = 0;
    
    ReleaseDC(NULL, hDC);
    return result;
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

#ifdef GLBINDINGS_GLEW
#else

#endif

    surfaces.insert(surf);

    return surf;
}

ISurface *
MSWinSessionManager::openScreen(int num, IDisplay *screen, ISurface::Attributes attribs)
{
    HWND hWnd;

    if (attribs.test(ISurface::SUPPORTS_OPENGL)) initOpenGL();

    int x, y;
    unsigned w, h;
    getScreenRect(num, x, y, w, h);

    MSWinSurface * surf = new MSWinSurface(screen);

    CreateParams cp;
    cp.surface = surf;
    cp.surf_attribs = attribs;
    cp.display = static_cast<IDisplay*>(screen);

    hWnd = CreateWindowEx( 0, WINDOW_CLASS_NAME
        , "GSM full-screen window" // TODO: caption != NULL ? caption : wclsname.c_str()
        , WS_POPUP | (attribs.test(ISurface::FORCE_FRAME) ? WS_BORDER : 0) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
        , x, y, w, h // position and size
        , NULL, NULL, NULL /*_module_instance()*/, &cp
        ); 
    if (hWnd == NULL) throw EMSWinError(GetLastError(), "CreateWindowEx");

    surfaces.insert(surf);

    return surf;
}

bool
MSWinSessionManager::fetchNextEvent()
{
    assert(!have_evt);

    for (surface_iterator_t it = surfaces.begin(); it != surfaces.end(); ++it) 
    {
        if (PeekMessage(&evt.msg, (*it)->hWnd, 0, 0, PM_REMOVE) ) 
        {
            if (evt.msg.message == WM_QUIT) {
                quit = true;
                return false;
            }
            else {
                have_evt = true;
#ifdef NOT_DEFINED
                if (TranslateMessage(&evt.msg))
                    return true;
                DispatchMessage(&evt.msg);
#else
                (void) TranslateMessage(&evt.msg);
                DispatchMessage(&evt.msg);
#endif
                return true;
            }
        }
    }
    return false;
}

IEvent *MSWinSessionManager::getEvent()
{
    if (have_evt) {
        have_evt = false;
        return &evt;
    }

    return nullptr;
}

IEvent *MSWinSessionManager::peekEvent()
{
    return have_evt ? &evt : nullptr;
}

bool MSWinSessionManager::mustQuit()
{
    return quit || surfaces.empty();
}

ISessionManager::MetaKeys
MSWinSessionManager::getMetaKeyStates()
{
    ISessionManager::MetaKeys keys;

    if ((GetKeyState(VK_SHIFT) & 0x8000) != 0)
        keys.set(SHIFT);
    if ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
        keys.set(CONTROL);
    if ((GetKeyState(VK_MENU) & 0x8000) != 0)
        keys.set(ALT);
	// For toggle keys, we're only interested in the toggle state
    if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
        keys.set(CAPSLOCK);
    if ((GetKeyState(VK_NUMLOCK) & 0x0001) != 0)
        keys.set(NUMLOCK);

    return keys;
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
