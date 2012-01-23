#ifndef __GSM_GFXRES_HPP
#define __GSM_GFXRES_HPP

namespace gsm {

    class IBoundGraphicsResource;

    class IGraphicsResource {
    public:
        virtual IBoundGraphicsResource * bind(int vidMemCtxID) = 0;
        virtual bool isBoundToContext(int vidMemCtxID) = 0;
        virtual void clearBindings() = 0;
    };

} // ns gsm

#endif // __GSM_GFXRES_HPP
