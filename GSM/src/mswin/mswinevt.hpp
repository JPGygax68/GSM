#ifndef __GSM_MSWINEVT_HPP
#define __GSM_MSWINEVT_HPP

#include "../../ievent.hpp"
#include "../../ikeybevt.hpp"
#include "../../iptrmovevt.hpp"
#include "../../iptrbtnevt.hpp"

namespace gsm {

    class ISurface;
    class IDisplay;

    class MSWinEvent: virtual public IEvent, virtual public IKeyboardEvent, 
        virtual public IPointerMotionEvent, virtual public IPointerButtonEvent 
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

    protected:
        MSG msg;
        friend class MSWinSessionManager;
    };

} // ns gsm

#endif // __GSM_MSWINEVT_HPP
