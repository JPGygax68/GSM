#ifndef __GSM_IEVENT_HPP
#define __GSM_IEVENT_HPP

namespace gsm {

    class IEvent {
    public:
        virtual bool 
        mustQuit() = 0;
    };

} // ns gsm

#endif // __GSM_IEVENT_HPP
