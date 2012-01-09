#ifndef __GSM_ISESSIONMGR_HPP
#define __GSM_ISESSIONMGR_HPP

#include "icomp.hpp"

namespace gsm {

class ISurface;
class IWindow;

class ISessionManager: public IComponent {
public:
    virtual ISurface *
    openWindow(int x, int y, int w, int h, IWindow *window) = 0;

    /** Returns true if an event has been processed (meaning that more might be pending).
     */
    virtual bool
    processNextEvent() = 0;

    virtual bool mustQuit() = 0;
};

} // ns gsm

#endif // __GSM_ISESSIONMGR_HPP
