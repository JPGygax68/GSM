#ifndef __GSM_IKEYBEVT_HPP
#define __GSM_IKEYBEVT_HPP

#include "ievent.hpp"

namespace gsm {

class IKeyboardEvent {
public:
    typedef unsigned long unicode_t; // TODO: does this really belong here?
    typedef int keycode_t;

    virtual bool down() = 0;
    virtual bool up() = 0;
    virtual bool isCharacter() = 0;
    virtual unicode_t unicode() = 0;
    virtual keycode_t keyCode() = 0;
};

} // ns gsm

#endif // __GSM_IKEYBEVT_HPP
