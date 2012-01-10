#ifndef __GSM_MSWINSURF_HPP
#define __GSM_MSWINSURF_HPP

#include <Windows.h>
#include "../../isurface.hpp"

namespace gsm {

    class MSWinSurface: public ISurface {
    public:
        ~MSWinSurface();

        virtual void show();

        virtual int clientWidth();
        virtual int clientHeight();

        virtual void select();

        HWND windowHandle() const { return hWnd; }

    protected:
        MSWinSurface(HWND hWnd);
    private:
        HWND hWnd;
        HGLRC hGLRC;
        friend class MSWinSessionManager;
    };

} // ns gsm

#endif // __GSM_MSWINSURF_HPP
