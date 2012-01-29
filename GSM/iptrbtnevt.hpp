#ifndef __GSM_IPTRBTNEVT_HPP
#define __GSM_IPTRBTNEVT_HPP

namespace gsm {

    class IPointerButtonEvent {
    public:
        enum Button { NONE = 0, LEFT, RIGHT, MIDDLE, WHEEL, EXTENDED_1, EXTENDED_2 };

        virtual const Position position() = 0;

        virtual Button button() = 0;
        virtual bool pressed() = 0;
        virtual bool released() = 0;
        virtual bool doubleClick() = 0;
    };

} // ns gsm

#endif // __GSM_IPTRBTNEVT_HPP
