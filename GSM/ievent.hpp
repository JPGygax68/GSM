/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_IEVENT_HPP
#define __GSM_IEVENT_HPP

#include "dll.h"

namespace gsm {

    class IDisplay;
    class IKeyboardEvent;
    class IPointerMotionEvent;
    class IPointerButtonEvent;
    class IMouseWheelEvent;

    class GSM_API IEvent {
    public:
        virtual IDisplay * target() = 0;

        virtual bool isQuit() = 0;
        virtual bool isKeyboard() = 0;
        virtual bool isPointerMotion() = 0;
        virtual bool isPointerButton() = 0;
        virtual bool isMouseWheel() = 0;
        virtual bool isCloseWindow() = 0;
        virtual bool isWindowSize() = 0;
        virtual bool isWindowManagement() = 0;

        virtual IKeyboardEvent *asKeyboard() = 0;
        virtual IPointerMotionEvent *asPointerMotion() = 0;
        virtual IPointerButtonEvent *asPointerButton() = 0;
        virtual IMouseWheelEvent *asMouseWheelEvent() = 0;

    };

} // ns gsm

#endif // __GSM_IEVENT_HPP
