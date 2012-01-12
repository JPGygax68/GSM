#ifndef __GSM_MSWINEVT_HPP
#define __GSM_MSWINEVT_HPP

#include "../../ievent.hpp"
#include "../../ikeybevt.hpp"

namespace gsm {

    class ISurface;
    class IWindow;

    class MSWinEvent: virtual public IEvent, virtual public IKeyboardEvent {
    
    public: // IEvent
        virtual IWindow * target();
        virtual bool isQuit();
        virtual bool isKeyboard();
        virtual bool isMouseMotion();
        virtual bool isMouseButton();
        virtual bool isCloseWindow();
        virtual bool isWindowSize();
        virtual bool isWindowManagement();
        virtual IKeyboardEvent * asKeyboard() { return this; }

    public: // IKeyboardEvent
        virtual bool down();
        virtual bool up();
        virtual bool isCharacter();
        virtual unicode_t asUnicode();
        virtual keycode_t asKeyCode();

    protected:
        MSG msg;
        friend class MSWinSessionManager;
    };

} // ns gsm

#endif // __GSM_MSWINEVT_HPP
