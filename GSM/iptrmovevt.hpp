#ifndef __GSM_IPTRMOVEVT_HPP
#define __GSM_IPTRMOVEVT_HPP

namespace gsm {

    class IPointerMotionEvent {
    public:
        typedef struct { int x, y; } Position;

        virtual const Position position() = 0;
    };

} // ns gsm

#endif // __GSM_IPTRMOVEVT_HPP
