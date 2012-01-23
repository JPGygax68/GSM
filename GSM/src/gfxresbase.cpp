#include <cassert>
#include "../gfxresbase.hpp"

namespace gsm {

void *
GraphicsResourceBase::bind(int vidMemCtxID)
{
    // Already bound ?
    bindings_t::iterator it = bindings.find(vidMemCtxID);
    if (it != bindings.end()) return it->second;

    // No, so we bind it, then register the binding
    void *res = doBind(vidMemCtxID);
    bindings.insert( bindings_t::value_type(vidMemCtxID, res) );

    return res;
}

bool GraphicsResourceBase::isBound(int vidMemCtxID)
{
    bindings_t::iterator it = bindings.find(vidMemCtxID);
    return it != bindings.end();
}

void GraphicsResourceBase::releaseBinding(int vidMemCtxID)
{
    bindings_t::iterator it = bindings.find(vidMemCtxID);
    assert(it != bindings.end());
    doRelease(vidMemCtxID, it->second);
}

} // ns gsm
