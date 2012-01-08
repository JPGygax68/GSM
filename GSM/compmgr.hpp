#ifndef __GSM_COMPMGR_HPP
#define __GSM_COMPMGR_HPP

#include <exception>
#include "util/format.hpp"

namespace gsm {

    class IComponent;

    IComponent *
    findComponent(const char *name);

    class ENoSuchComponent: public std::exception {
    public:
        ENoSuchComponent(const char *name):
          std::exception(
              format("GSM Component Manager: No component was registered under the name \"%s\"", name).c_str()) {}
    };

} // ns gsm

#endif // __GSM_COMPMGR_HPP
