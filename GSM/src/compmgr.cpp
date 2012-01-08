#include <string>
#include <map>
#include "../compmgr.hpp"

namespace gsm {

typedef std::map<std::string, IComponent*> component_map_t;
typedef component_map_t::value_type compmap_value_t;
typedef component_map_t::iterator compmap_iter_t;

static component_map_t *
repository = NULL;

component_map_t * getRepository();

IComponent *
findComponent(const char *name)
{
    component_map_t *rep = getRepository();

    compmap_iter_t iter = rep->find(name);
    if (iter == rep->end())
        throw ENoSuchComponent(name);

    return iter->second;
}

static component_map_t *
getRepository()
{
    if (repository == NULL) {
        repository = new component_map_t;
    }
    return repository;
}

} // ns gsm
