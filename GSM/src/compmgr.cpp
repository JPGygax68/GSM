#include <string>
#include <map>
#include "../icomp.hpp"
#include "../compmgr.hpp"

namespace gsm {

typedef std::map<std::string, IComponent*> component_map_t;
typedef component_map_t::value_type compmap_value_t;
typedef component_map_t::iterator compmap_iter_t;

static component_map_t *
repository = NULL;

component_map_t * getRepository();

//--- PUBLIC FUNCTIONS --------------------------------------------------------

IComponent * 
findComponent(const char *name)
{
    component_map_t *rep = getRepository();

    compmap_iter_t iter = rep->find(name);
    if (iter == rep->end())
        throw ENoSuchComponent(name);

    return iter->second;
}

void
registerComponent(const char *name, IComponent *component)
{
    component_map_t *rep = getRepository();

    if (rep->find(name) != rep->end())
        throw EComponentAlreadyRegistered(name);

    rep->insert( compmap_value_t(name, component) );
}

void
unregisterComponent(IComponent *component)
{
    component_map_t *rep = getRepository();

    for (compmap_iter_t it = rep->begin(); it != rep->end(); it++) {
        if (it->second == component) {
            rep->erase(it);
            return;
        }
    }

    throw EComponentNotRegistered();
}

void
unregisterAll()
{
    component_map_t *rep = getRepository();

    for (compmap_iter_t it = rep->begin(); it != rep->end(); it++) {
        delete it->second;
    }

    rep->clear();
}

//--- PRIVATE FUNCTIONS ------------------------------------------------------

static component_map_t *
getRepository()
{
    if (repository == NULL) {
        repository = new component_map_t;
    }
    return repository;
}

//--- EXCEPTIONS --------------------------------------------------------------

EComponentManager::EComponentManager(const char *msg)
    : std::exception( (std::string("GSM Component Manager error: ")+msg).c_str() )
{}

ENoSuchComponent::ENoSuchComponent(const char *name)
    : EComponentManager(format("No component was registered under the name \"%s\"", name).c_str())
{}

EComponentAlreadyRegistered::EComponentAlreadyRegistered(const char *name)
    : EComponentManager(format("Component \"%s\" has already been registered", name).c_str())
{}

EComponentNotRegistered::EComponentNotRegistered()
    : EComponentManager("Tried to remove a component that was not in the repository")
{}

} // ns gsm
