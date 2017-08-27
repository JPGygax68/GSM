/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_MSWINEVT_HPP
#define __GSM_MSWINEVT_HPP

#include "../../ievent.hpp"
#include "../../ikeybevt.hpp"
#include "../../iptrmovevt.hpp"
#include "../../iptrbtnevt.hpp"
#include "../../imousewhlevt.hpp"

namespace gsm {

    class ISurface;
    class IDisplay;

    class MSWinEvent: public IEvent, public IKeyboardEvent, 
        public IPointerMotionEvent, public IPointerButtonEvent, public IMouseWheelEvent
    {
    
    public: // IEvent
        virtual IDisplay * target();
        bool isQuit() override;
        bool isKeyboard() override;
        bool isPointerMotion() override;
        bool isPointerButton() override;
        bool isMouseWheel() override;
        bool isCloseWindow() override;
        bool isWindowSize() override;
        bool isWindowManagement() override;
        IKeyboardEvent * asKeyboard() override { return this; }
        IPointerMotionEvent *asPointerMotion() override { return this; }
        IPointerButtonEvent *asPointerButton() override { return this; }
        IMouseWheelEvent *asMouseWheelEvent() override { return this; }

    public: // IKeyboardEvent
        bool down() override;
        bool up() override;
        bool isCharacter() override;
        unicode_t unicode() override;
        keycode_t keyCode() override;

    public: // IPointerMotionEvent
        const Position position() override;

    public: // IPointerButtonEvent
        Button button() override;
        bool pressed() override;
        bool released() override;
        bool doubleClick() override;

    public: // IMouseWheelEvent
        int delta() override;

    protected:
        MSG msg;
        friend class MSWinSessionManager;
    };

} // ns gsm

#endif // __GSM_MSWINEVT_HPP
