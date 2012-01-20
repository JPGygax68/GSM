#ifndef __GSM_ISESSIONMGR_HPP
#define __GSM_ISESSIONMGR_HPP

#include "icomp.hpp"
#include "isurface.hpp"
#include "util/bitset.hpp"

namespace gsm {

class ISurface;
class IWindow;
class IScreen;
class IEvent;

class ISessionManager: public IComponent {
public:

    enum MetaKey {
        SHIFT, CONTROL, ALT, CAPSLOCK, NUMLOCK
    };
    typedef bitset<MetaKey> MetaKeys;

    virtual ISurface *
    openWindow(int x, int y, int w, int h, const char *caption, IWindow *window, ISurface::Attributes attribs = 0) = 0;

    virtual ISurface *
    openScreen(int num, ISurface::Attributes attr, IScreen *screen, ISurface::Attributes attribs = 0) = 0;

    /** Returns true if an event has become available (meaning that more might be pending).
     */
    virtual bool fetchNextEvent() = 0;

    /** Can only be called if a preceding call to processPendingEvents() returned true.
        After this call, the event be handled completely before processPendingEvents()
        is called again.
     */
    virtual IEvent * getEvent() = 0;

    // TODO: define a dispatchEvent() and / or a processPendingEvents() ?

    virtual bool mustQuit() = 0;

    virtual bitset<MetaKey> getMetaKeyStates() = 0;
};

} // ns gsm

#endif // __GSM_ISESSIONMGR_HPP
