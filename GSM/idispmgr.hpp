#ifndef __GSM_IDISPMGR_HPP
#define __GSM_IDISPMGR_HPP

#include "icomp.hpp"

namespace gsm {

class ISurface;

class IDisplayManager: public IComponent {
public:
    virtual ISurface *
    openWindow() = 0;
};

} // ns gsm

#endif // __GSM_IDISPMGR_HPP
