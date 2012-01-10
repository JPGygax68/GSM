#include "mswinsurf.hpp"
#include "mswinerr.hpp"

namespace gsm {

MSWinSurface::MSWinSurface(HWND hWnd_)
{
    hWnd = hWnd_;
    hGLRC = 0;
}

MSWinSurface::~MSWinSurface()
{
    if (! CloseWindow(hWnd) )
        throw EMSWinError(GetLastError(), "CloseWindow");
}

void
MSWinSurface::show()
{
    ShowWindow(hWnd, SW_SHOW);
}

int
MSWinSurface::clientWidth()
{
    RECT r;
    CHECK(GetClientRect, (hWnd, &r));
    return r.right - r.left;
}

int
MSWinSurface::clientHeight()
{
    RECT r;
    CHECK(GetClientRect, (hWnd, &r));
    return r.bottom - r.top;
}

void
MSWinSurface::select()
{
    if (hGLRC != 0) {
        CHECK(wglMakeCurrent, (GetDC(hWnd), hGLRC));
    }
}

void
MSWinSurface::setCaption(const char *cap)
{
    if (hGLRC != 0) {
        SetWindowText(hWnd, cap);
    }
}

} // ns mswinsurf
