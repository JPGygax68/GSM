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

namespace gsm {

    class ISurface;
    class IDisplay;

    class MSWinEvent: public IEvent, public IKeyboardEvent, 
        public IPointerMotionEvent, public IPointerButtonEvent 
    {
    
    public: // IEvent
        virtual IDisplay * target();
        virtual bool isQuit();
        virtual bool isKeyboard();
        virtual bool isPointerMotion();
        virtual bool isPointerButton();
        virtual bool isCloseWindow();
        virtual bool isWindowSize();
        virtual bool isWindowManagement();
        virtual IKeyboardEvent * asKeyboard() { return this; }
        virtual IPointerMotionEvent *asPointerMotion() { return this; }
        virtual IPointerButtonEvent *asPointerButton() { return this; }

    public: // IKeyboardEvent
        virtual bool down();
        virtual bool up();
        virtual bool isCharacter();
        virtual unicode_t unicode();
        virtual keycode_t keyCode();

    public: // IPointerMotionEvent
        virtual const Position position();

    public: // IPointerButtonEvent
        virtual Button button();
        virtual bool pressed();
        virtual bool released();
        virtual bool doubleClick();

    protected:
        MSG msg;
        friend class MSWinSessionManager;
    };

} // ns gsm

#endif // __GSM_MSWINEVT_HPP
