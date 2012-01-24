#ifndef __GSM_MSWINSURF_HPP
#define __GSM_MSWINSURF_HPP

#include <Windows.h>
#include "../../isurface.hpp"

namespace gsm {

    class MSWinSurface: public ISurface {
    public:
        ~MSWinSurface();

    public: // ISurface
        virtual void show();

        virtual int clientWidth();
        virtual int clientHeight();

        virtual int
        select(int monitor = 0, Rect *rect = NULL);

        virtual void setCaption(const char *cap);

        virtual void present(int monitor = 0);

        /** Returns the main Video Context ID.
            Note that a Surface can spread across several monitors, which may not be
            connected to the same graphics adapter, in which case several Video
            Contexts are required to fully render the Surface.
         */
        virtual int videoContextID() { return vidctx_id; }

    public:
        HWND windowHandle() const { return hWnd; }
        void setWindowHandle(HWND hWnd_) { hWnd = hWnd_; }

        HGLRC openGLContext() const { return hGLRC; }
        void setOpenGLContext(HGLRC hRC) { hGLRC = hRC; }

        void setVideoContextID(int id) { vidctx_id = id; }

    protected:
        MSWinSurface();
    
    private:
        void bindGraphicsResources();

        HWND    hWnd;
        HGLRC   hGLRC;
        int     vidctx_id;
        friend class MSWinSessionManager;
    };

} // ns gsm

#endif // __GSM_MSWINSURF_HPP
