#include <Windows.h>
#include "mswinevt.hpp"

namespace gsm {

    IWindow *
    MSWinEvent::target()
    {
        return reinterpret_cast<IWindow*>(GetWindowLongPtr(msg.hwnd, GWLP_USERDATA)); 
    }

    const bool
    MSWinEvent::isQuit()
    {
        return msg.message == WM_QUIT;
    }

    const bool
    MSWinEvent::isKeyboard()
    {
        WORD code = LOWORD(msg.message);
        return code >= WM_KEYFIRST && code <= WM_KEYLAST;
    }

    const bool
    MSWinEvent::isMouseMotion()
    { 
        return LOWORD(msg.message) == WM_MOUSEMOVE;
    }

    const bool
    MSWinEvent::isMouseButton()
    {
        WORD code = LOWORD(msg.message);
        return (code >= WM_LBUTTONDOWN && code <= WM_MBUTTONDBLCLK)
            || (code >= WM_XBUTTONDOWN && code <= WM_XBUTTONDBLCLK);
    }

    const bool
    MSWinEvent::isCloseWindow()
    {
        return (msg.message == WM_CLOSE)
            || (msg.message == WM_SYSCOMMAND && msg.wParam == SC_CLOSE);
    }

    const bool
    MSWinEvent::isWindowSize()
    {
        return (msg.message == WM_SIZE);
    }

    const bool 
    MSWinEvent::isWindowManagement()
    {
        WORD code = LOWORD(msg.message);
        return code == WM_QUIT || isCloseWindow();
    }

} // ns gsm
