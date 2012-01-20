#ifndef __GSM_ISURFACE_HPP
#define __GSM_ISURFACE_HPP

#include "util/bitset.hpp"

namespace gsm {
    
class IEvent;

class ISurface {
public:

    virtual ~ISurface() {}

    enum Attribute {
        SINGLE_BUFFERED,
        SUPPORTS_OPENGL
    };
    typedef bitset<Attribute> Attributes;

    virtual void
    show() = 0;

    /** Must return the width of the usable part of the surface (without the frame if there is one).
     */
    virtual int
    clientWidth() = 0;

    /** Must return the height of the usable part of the surface (without the frame if there is one).
     */
    virtual int
    clientHeight() = 0;

    /** Graphic APIs like OpenGL work within contexts. By definition, there is one such context per
        Surface, which the call select() is responsible for activating. However, graphical resources
        can sometimes be shared between contexts. In order for user code to know when it needs to
        re-generate a given graphical resource, it must be able to identify the currently active
        "resource pool". This call returns an opaque ID fulfilling that purpose.
     */
    virtual int 
    resourcePoolID() = 0;

    /** Prepares the surface for use by the graphics API (typically OpenGL).
     */
    virtual void
    select() = 0;

    virtual void
    setCaption(const char *cap) = 0;

    /** Signals that painting on the surface is done and that it can be presented on the monitor(s).
     */
    virtual void
    present() = 0;
};

} // ns gsm

#endif // __GSM_ISURFACE_HPP
