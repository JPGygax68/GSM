#ifndef __GSM_ISESSIONMGR_HPP
#define __GSM_ISESSIONMGR_HPP

#include "icomp.hpp"

namespace gsm {

class ISurface;

class ISessionManager: public IComponent {
public:
    virtual ISurface *
    openWindow() = 0;

    virtual void
    run() = 0;
};

} // ns gsm

#endif // __GSM_ISESSIONMGR_HPP
