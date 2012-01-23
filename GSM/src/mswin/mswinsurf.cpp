#include "mswinerr.hpp"
#include "mswinogl.hpp"
#include "mswinsurf.hpp"

namespace gsm {

MSWinSurface::MSWinSurface(HWND hWnd_)
{
    hWnd = hWnd_;
    hGLRC = 0;
    vidctx_id = 0;
}

MSWinSurface::~MSWinSurface()
{
    if (! CloseWindow(hWnd) )
        throw EMSWinError(GetLastError(), "CloseWindow");

    if (hGLRC != 0) {
        retireContext(hGLRC);
    }
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

int
MSWinSurface::select(int monitor, Rect *rect)
{
    // TODO: support multiple monitors on XP
    if (hGLRC != 0) {
        CHECK(wglMakeCurrent, (GetDC(hWnd), hGLRC));
    }
    return 1;
}

void
MSWinSurface::setCaption(const char *cap)
{
    SetWindowText(hWnd, cap);
}

void
MSWinSurface::present(int monitor)
{
    if (hGLRC != 0) {
        SwapBuffers(GetDC(hWnd));
    }
}

/*
void
MSWinSurface::bindGraphicsResources()
{
    for (unsigned i = 0; i < gfx
}
*/

} // ns mswinsurf
