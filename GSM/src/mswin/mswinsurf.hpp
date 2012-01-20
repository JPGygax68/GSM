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

        virtual void select();

        virtual void setCaption(const char *cap);

        virtual void present();

        virtual int resourcePoolID() { return 0; } // TODO!

    public:
        HWND windowHandle() const { return hWnd; }

        HGLRC openGLContext() const { return hGLRC; }
        void setOpenGLContext(HGLRC hRC) { hGLRC = hRC; }

    protected:
        MSWinSurface(HWND hWnd);
    private:
        HWND hWnd;
        HGLRC hGLRC;
        friend class MSWinSessionManager;
    };

} // ns gsm

#endif // __GSM_MSWINSURF_HPP
