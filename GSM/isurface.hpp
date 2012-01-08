#ifndef __GSM_ISURFACE_HPP
#define __GSM_ISURFACE_HPP

namespace gsm {
    
    class IEvent;

    class ISurface {
    public:

        virtual void
        show() = 0;
    };

} // ns gsm

#endif // __GSM_ISURFACE_HPP
