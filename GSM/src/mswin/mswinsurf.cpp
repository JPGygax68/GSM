/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#include <cassert>
#include "../../util/oglhelper.hpp"
#include "mswinerr.hpp"
#include "mswinogl.hpp"
#include "mswinsurf.hpp"

namespace gsm {

MSWinSurface::MSWinSurface(IDisplay *disp_)
{
    disp = disp_;
    hWnd = 0;
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
        HDC hDC = GetDC(hWnd);
        //OutputDebugString(format("MSWinSurface::select() with hGLRC = %x\n", hGLRC).c_str() );
        CHECK(wglMakeCurrent, (hDC, hGLRC));
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
        HDC hDC = GetDC(hWnd);
        CHECK(wglMakeCurrent, (0, 0));
        CHECK(SwapBuffers, (hDC));
		//OGL(glFinish, ());
		//OGL(glFlush, ());
        //wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
    }
}

void *
MSWinSurface::createExtraContext()
{
	assert(hGLRC != 0);
	HDC hDC = GetDC(hWnd);
	HGLRC hRC = wglCreateContext(hDC);
	if (hRC == 0) throw EMSWinError(GetLastError(), "wglCreateContext() (extra context)");
	// TODO: share lists
	return hRC;
}

void
MSWinSurface::selectExtraContext(void *ctx)
{
	HDC hDC = GetDC(hWnd);
	CHECK(wglMakeCurrent, (hDC, (HGLRC)ctx));
}

void
MSWinSurface::deleteExtraContext(void *ctx)
{
	CHECK(wglDeleteContext, ((HGLRC)ctx));
}

} // ns mswinsurf
