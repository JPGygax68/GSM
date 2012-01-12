#include <Windows.h>
#include "mswinevt.hpp"

namespace gsm {

    //--- IEvent --------------------------------------------------------------

    IDisplay *
    MSWinEvent::target()
    {
        return reinterpret_cast<IDisplay*>(GetWindowLongPtr(msg.hwnd, GWLP_USERDATA)); 
    }

    bool
    MSWinEvent::isQuit()
    {
        return msg.message == WM_QUIT;
    }

    bool
    MSWinEvent::isKeyboard()
    {
        WORD code = LOWORD(msg.message);
        return code >= WM_KEYFIRST && code <= WM_KEYLAST;
    }

    bool
    MSWinEvent::isPointerMotion()
    { 
        return LOWORD(msg.message) == WM_MOUSEMOVE;
    }

    bool
    MSWinEvent::isPointerButton()
    {
        WORD code = LOWORD(msg.message);
        return (code >= WM_LBUTTONDOWN && code <= WM_MBUTTONDBLCLK)
            || (code >= WM_XBUTTONDOWN && code <= WM_XBUTTONDBLCLK);
    }

    bool
    MSWinEvent::isCloseWindow()
    {
        return (msg.message == WM_CLOSE)
            || (msg.message == WM_SYSCOMMAND && msg.wParam == SC_CLOSE);
    }

    bool
    MSWinEvent::isWindowSize()
    {
        return (msg.message == WM_SIZE);
    }

    bool 
    MSWinEvent::isWindowManagement()
    {
        WORD code = LOWORD(msg.message);
        return code == WM_QUIT || isCloseWindow();
    }

    //--- IKeyboardEvent ------------------------------------------------------

   bool
   MSWinEvent::down()
   {
        return msg.message == WM_CHAR    || msg.message == WM_UNICHAR
            || msg.message == WM_KEYDOWN || msg.message == WM_SYSKEYDOWN;
   }

   bool
   MSWinEvent::up()
   {
        return msg.message == WM_KEYUP || msg.message == WM_SYSKEYUP;
   }

   bool
   MSWinEvent::isCharacter()
   {
        return msg.message == WM_CHAR || msg.message == WM_UNICHAR;
   }

   IKeyboardEvent::unicode_t
   MSWinEvent::unicode()
   {
        return (unicode_t) msg.wParam; 
   }

   IKeyboardEvent::keycode_t
   MSWinEvent::keyCode()
   {
        return msg.wParam;
   }


} // ns gsm
