#ifndef __GSM_GFXRESBASE_HPP
#define __GSM_GFXRESBASE_HPP

#include <set>
#include "igfxres.hpp"

namespace gsm {

    class GraphicsResourceBase : public IGraphicsResource {
    public:
        virtual bool isBoundToContext(int vidMemCtxID);
        virtual void clearBindings();
    protected:
        bool registerBinding(int vidMemCtxID);
    private:
        std::set<int> bindings;
    };

} // ns gsm

#endif // __GSM_GFXRESBASE_HPP
