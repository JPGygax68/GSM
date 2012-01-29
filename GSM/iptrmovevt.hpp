#ifndef __GSM_IPTRMOVEVT_HPP
#define __GSM_IPTRMOVEVT_HPP

#include "gfxtypes.hpp"

namespace gsm {

    class IPointerMotionEvent {
    public:

        virtual const Position position() = 0;
    };

} // ns gsm

#endif // __GSM_IPTRMOVEVT_HPP
