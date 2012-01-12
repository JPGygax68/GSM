#ifndef __GSM_IEVENT_HPP
#define __GSM_IEVENT_HPP

namespace gsm {

    class IWindow;

    class IEvent {
    public:
        virtual IWindow * target() = 0;

        virtual const bool isQuit() = 0;

        virtual const bool isKeyboard() = 0;

        virtual const bool isMouseMotion() = 0;

        virtual const bool isMouseButton() = 0;

        virtual const bool isCloseWindow() = 0;

        virtual const bool isWindowSize() = 0;
        
        virtual const bool isWindowManagement() = 0;

    };

} // ns gsm

#endif // __GSM_IEVENT_HPP
