#ifndef __GSM_ISURFACE_HPP
#define __GSM_ISURFACE_HPP

#include "util/bitset.hpp"

namespace gsm {
    
class IEvent;

class ISurface {
public:

    enum Attribute {
        SINGLE_BUFFERED
    };
    typedef bitset<Attribute> Attributes;

    virtual void
    show() = 0;

    virtual int
    clientWidth() = 0;

    virtual int
    clientHeight() = 0;

    /** Prepares the surface for use by the graphics API (typically OpenGL).
     */
    virtual void
    select() = 0;
};

} // ns gsm

#endif // __GSM_ISURFACE_HPP
