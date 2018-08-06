/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#include <Windowsx.h>
#include "mswinsurf.hpp"
#include "mswinevt.hpp"

namespace gsm {

    //--- IEvent --------------------------------------------------------------

    IDisplay *
    MSWinEvent::target()
    {
        MSWinSurface *surf = reinterpret_cast<MSWinSurface*>(GetWindowLongPtr(msg.hwnd, GWLP_USERDATA));
        if (surf == NULL) return 0;
        return surf->display();
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

    bool MSWinEvent::isMouseWheel()
    {
        return LOWORD(msg.message) == WM_MOUSEWHEEL;
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

    unicode_t
    MSWinEvent::unicode()
    {
        return (unicode_t) msg.wParam; 
    }

    IKeyboardEvent::keycode_t
    MSWinEvent::keyCode()
    {
        return msg.wParam;
    }

    const Position
    MSWinEvent::position()
    {
        POINT p;
        p.x = GET_X_LPARAM(msg.lParam);
        p.y = GET_Y_LPARAM(msg.lParam);
        if (msg.message == WM_MOUSEWHEEL) {
            (void)MapWindowPoints(HWND_DESKTOP, msg.hwnd, &p, 1);
        }
        return Position{ p.x, p.y };
    }

    IPointerButtonEvent::Button
    MSWinEvent::button()
    {
        switch ( msg.message ) {
	        case WM_LBUTTONDOWN: 
	        case WM_LBUTTONUP: 
	        case WM_LBUTTONDBLCLK:
		        return LEFT; 
	        case WM_MBUTTONDOWN:
	        case WM_MBUTTONUP: 
	        case WM_MBUTTONDBLCLK:
		        return MIDDLE;
	        case WM_RBUTTONDOWN:
	        case WM_RBUTTONUP: 
	        case WM_RBUTTONDBLCLK:
		        return RIGHT; 
	        case WM_XBUTTONDOWN:
	        case WM_XBUTTONUP:
		        switch ( HIWORD(msg.wParam) ) {
			        case XBUTTON1: return EXTENDED_1;
			        case XBUTTON2: return EXTENDED_2;
			        default: return NONE;
		        }
	        default:
		        return NONE;
        };
    }

    bool
    MSWinEvent::pressed()
    {
        switch ( msg.message ) {
	        case WM_LBUTTONDOWN: 
	        case WM_MBUTTONDOWN:
	        case WM_RBUTTONDOWN:
	        case WM_XBUTTONDOWN:
		        return true;
	        default:
		        return false;
        }
    }

    bool
    MSWinEvent::released()
    {
        switch ( msg.message ) {
	        case WM_LBUTTONUP: 
	        case WM_MBUTTONUP:
	        case WM_RBUTTONUP:
		        return true;
	        default:
		        return false;
        }
    }

    bool
    MSWinEvent::doubleClick()
    {
        switch ( msg.message ) {
	        case WM_LBUTTONDBLCLK:
	        case WM_MBUTTONDBLCLK:
	        case WM_RBUTTONDBLCLK:
		        return true;
	        default:
		        return false;
        }
    }

    int MSWinEvent::delta()
    {
        uint16_t hw = HIWORD(msg.wParam);
        return *reinterpret_cast<int16_t*>(&hw);
    }

} // ns gsm
