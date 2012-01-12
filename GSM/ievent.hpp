#ifndef __GSM_IEVENT_HPP
#define __GSM_IEVENT_HPP

namespace gsm {

    class IWindow;

    class IEvent {
    public:
        virtual IWindow * target() = 0;

        virtual bool isQuit() = 0;

        virtual bool isKeyboard() = 0;

        virtual bool isMouseMotion() = 0;

        virtual bool isMouseButton() = 0;

        virtual bool isCloseWindow() = 0;

        virtual bool isWindowSize() = 0;
        
        virtual bool isWindowManagement() = 0;

    };

} // ns gsm

#endif // __GSM_IEVENT_HPP
