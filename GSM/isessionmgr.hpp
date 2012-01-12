#ifndef __GSM_ISESSIONMGR_HPP
#define __GSM_ISESSIONMGR_HPP

#include "icomp.hpp"
#include "isurface.hpp"

namespace gsm {

class ISurface;
class IWindow;
class IScreen;
class IEvent;

class ISessionManager: public IComponent {
public:
    virtual ISurface *
    openWindow(int x, int y, int w, int h, const char *caption, IWindow *window) = 0;

    virtual ISurface *
    openScreen(int num, ISurface::Attributes attr, IScreen *screen) = 0;

    /** Returns true if an event has been processed (meaning that more might be pending).
     */
    virtual bool
    processPendingEvents() = 0;

    /** Can only be called if a preceding call to processPendingEvents() returned true.
        After this call, the event be handled completely before processPendingEvents()
        is called again.
     */
    virtual IEvent *
    getEvent() = 0;

    virtual bool mustQuit() = 0;
};

} // ns gsm

#endif // __GSM_ISESSIONMGR_HPP
