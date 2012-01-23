#include "../gfxresbase.hpp"

namespace gsm {

bool GraphicsResourceBase::isBoundToContext(int vidMemCtxID)
{
    std::set<int>::iterator it = bindings.find(vidMemCtxID);
    return it != bindings.end();
}

void GraphicsResourceBase::clearBindings()
{
    bindings.clear();
}

bool GraphicsResourceBase::registerBinding(int vidMemCtxID)
{
    // Resource was already bound to this video memory context?
    std::set<int>::iterator it = bindings.find(vidMemCtxID);
    if (it != bindings.end()) return true;
    // No, add the context to the set
    bindings.insert(vidMemCtxID);
    return false;
}

} // ns gsm
