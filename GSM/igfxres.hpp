#ifndef __GSM_GFXRES_HPP
#define __GSM_GFXRES_HPP

namespace gsm {

    class IBoundGraphicsResource;

    class IGraphicsResource {
    public:
        virtual void * bind(int vidMemCtxID) = 0;
        virtual bool isBound(int vidMemCtxID) = 0;
        virtual void releaseBinding(void *bgres) = 0;
    };

} // ns gsm

#endif // __GSM_GFXRES_HPP
