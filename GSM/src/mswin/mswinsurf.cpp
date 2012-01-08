#include "mswinsurf.hpp"
#include "mswinerr.hpp"

namespace gsm {

MSWinSurface::MSWinSurface(HWND hWnd_)
{
    hWnd = hWnd_;
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

} // ns mswinsurf
