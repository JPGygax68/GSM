#ifndef __GSM_MSWINEVT_HPP
#define __GSM_MSWINEVT_HPP

#include "../../ievent.hpp"

namespace gsm {

    class ISurface;
    class IWindow;

    class MSWinEvent: virtual public IEvent {
    public:
        virtual IWindow * target();

        virtual const bool isQuit();

        virtual const bool isKeyboard();

        virtual const bool isMouseMotion();

        virtual const bool isMouseButton();

        virtual const bool isCloseWindow();

        virtual const bool isWindowSize();
        
        virtual const bool isWindowManagement();
        
    private:
        MSG msg;
        friend class MSWinSessionManager;
    };

} // ns gsm

#endif // __GSM_MSWINEVT_HPP
