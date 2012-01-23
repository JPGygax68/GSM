#ifndef __GSM_GFXRESBASE_HPP
#define __GSM_GFXRESBASE_HPP

#include <map>
#include "igfxres.hpp"

namespace gsm {

    class GraphicsResourceBase : public IGraphicsResource {
    public:
        virtual void *bind(int vidMemCtxID);
        virtual bool isBound(int vidMemCtxID);
        virtual void releaseBinding(int vidMemCtxID);
    protected:
        virtual void *doBind(int vidMemCtxID) = 0;
        virtual void doRelease(int vidMemCtxID, void *bgres) = 0;
    private:
        typedef std::map<int, void*> bindings_t;
        bindings_t bindings;
    };

} // ns gsm

#endif // __GSM_GFXRESBASE_HPP
