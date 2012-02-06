/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_COMPMGR_HPP
#define __GSM_COMPMGR_HPP

#include <exception>
#include "util/format.hpp"

namespace gsm {

    class IComponent;

    IComponent *
    findComponent(const char *name);

    void
    registerComponent(const char *name, IComponent *comp);

    void
    unregisterComponent(IComponent *comp);

    void
    unregisterAll();

    class EComponentManager: public std::exception {
    protected:
        EComponentManager(const char *msg);
    };

    class ENoSuchComponent: public EComponentManager {
    public:
        ENoSuchComponent(const char *name);
    };

    class EComponentAlreadyRegistered: public EComponentManager {
    public:
        EComponentAlreadyRegistered(const char *name);
    };

    class EComponentNotRegistered: public EComponentManager {
    public:
        EComponentNotRegistered();
    };

} // ns gsm

#define REGISTER_COMPONENT(compname, compclass) \
    static class Registrator { \
    public: \
        Registrator() { \
            instance = new compclass(); \
            registerComponent(compname, instance); } \
        ~Registrator() { \
            unregisterComponent(instance); \
            delete instance; } \
    private: \
        compclass * instance; \
    } registrator; \

#endif // __GSM_COMPMGR_HPP
