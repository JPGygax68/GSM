#include <Windows.h>
#include "../../isessionmgr.hpp"
#include "../../compmgr.hpp"
#include "../../util/format.hpp"
#include "mswinerr.hpp"
#include "mswinsurf.hpp"

namespace gsm {

class MSWinSessionManager: public ISessionManager {
private:
    MSWinSessionManager();
    ~MSWinSessionManager();
    friend class Registrator;

public:
    virtual ISurface *
    openWindow();

    virtual void
    run();
};

REGISTER_COMPONENT("SessionManager", MSWinSessionManager);

//--- INTERNAL CONSTANTS ------------------------------------------------------

static const char *WINDOW_CLASS_NAME = "GSM Window Class";

//--- PRIVATE FUNCTIONS -------------------------------------------------------

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
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
		//OutputDebugString(L"WM_ERASEBKGND\n");
		return 0;
		break;
	case WM_SIZE:
        // TODO
		break;
	case WM_CLOSE:
        // TODO
		break;
	case WM_PAINT: 
		// TODO
		break;
	case WM_SYSCOMMAND:
        // TODO
        /*
		if (it != windows().end() ) {
			switch (wParam) {    
			case SC_KEYMENU:
				return 0;
			}
		}
        */
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

//--- DISPLAY MANAGER IMPLEMENTATION ------------------------------------------

MSWinSessionManager::MSWinSessionManager()
{
    registerWindowClass();
}

MSWinSessionManager::~MSWinSessionManager()
{
    unregisterWindowClass();
}

ISurface *
MSWinSessionManager::openWindow()
{
    HWND hWnd;

    hWnd = CreateWindowEx( 0, WINDOW_CLASS_NAME
        , "GSM Window" // TODO: caption != NULL ? caption : wclsname.c_str()
        , WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
        , 0, 0, 0, 0 // position and size
        , NULL, NULL, NULL /*_module_instance()*/, NULL
        ); 
    if (hWnd == NULL)
        throw EMSWinError(GetLastError(), "CreateWindowEx");

    return new MSWinSurface(hWnd);
}

void
MSWinSessionManager::run()
{
    // TODO
}

} // ns gsm
