/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#include <vector>
#include <cassert>
#include <GL/glew.h>
#include <GL/wglew.h>

#include "mswinerr.hpp"
#include "mswinsurf.hpp"
#include "mswinogl.hpp"

#pragma comment (lib, "glew32.lib")

namespace gsm {

//--- PRIVATE TYPES ----------------------------------------------------------

typedef std::vector<HGLRC>           video_context_t;
typedef std::vector<video_context_t> video_contexts_t;

//--- PRIVATE DATA -----------------------------------------------------------

static video_contexts_t     video_contexts;

//--- PRIVATE ROUTINES -------------------------------------------------------

static int
selectPreliminaryPixelFormat(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW 
        | PFD_SUPPORT_OPENGL
        | PFD_GENERIC_ACCELERATED
        ;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    return ChoosePixelFormat(hDC, &pfd);
}

/** 
    TODO: more parameters!
 */
static int
selectPixelFormat(HDC hDC, bool double_buffered)
{
    // Try to select a format that supports multisampling
    int pixelFormats[32];
    BOOL bStatus;
    UINT numFormats;
    float fAttributes[] = {0,0};
    int iAttributes[] = { 
        WGL_DRAW_TO_WINDOW_ARB,		GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,		GL_TRUE,
        WGL_ACCELERATION_ARB,		WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB,			24,
        WGL_ALPHA_BITS_ARB,			8,
        WGL_DEPTH_BITS_ARB,			16,
        WGL_STENCIL_BITS_ARB,		0,
        WGL_DOUBLE_BUFFER_ARB,		double_buffered ? GL_TRUE : GL_FALSE,
        // Multisampling only works when double-buffering is enabled
        WGL_SAMPLE_BUFFERS_ARB,		double_buffered ? GL_TRUE : GL_FALSE,
        WGL_SAMPLES_ARB,			double_buffered ? 4 : 0,
        0, 0
    };
	// Multisampling ?
    if (iAttributes[17] == GL_TRUE) {
        // TODO: make sample count configurable
        for (unsigned samples = 4; samples > 0; samples -= 2) {
            iAttributes[19] = samples;
            bStatus = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes,1, pixelFormats, &numFormats);
            if (bStatus == GL_TRUE && numFormats > 0) {
                return pixelFormats[0];
            }
        }
    }
    else {
        bStatus = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes,1, pixelFormats, &numFormats);
        if (bStatus == GL_TRUE && numFormats > 0)
            return pixelFormats[0];
    }
    // Log a warning here
    OutputDebugString("selectPixelFormat(): wglChoosePixelFormatARB() failed, falling back to ChoosePixelFormat\n");

    // Fall back to standard method
    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED
        | (double_buffered ? PFD_DOUBLEBUFFER : 0);
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    return ChoosePixelFormat(hDC, &pfd);
}

static int
assignToVideoContext(HGLRC hRC)
{
    // Traverse existing Video Contexts to see if we can share gfx resources with any of them
    video_contexts_t::iterator it = video_contexts.begin();
    int i = 0;
	int iempty = -1;
    for ( ; it != video_contexts.end(); it ++, i++) 
    {
		if (!it->empty()) {
			// Try to share resource lists with first Context in the group
			if (wglShareLists(it->front(), hRC) == TRUE) {
				// Succeeded, so add the new Rendering Context to this Video Context
				it->push_back(hRC);
				// We're done, this is the Video Context we were looking for
				return 1 + i;
			}
		}
		else if (iempty < 0) {
			iempty = i; // remember this Video Context as available
		}
    }
    // Could not share with any Video Context, so create and add new Video Context (or reuse an empty one)
	if (iempty < 0) {
		video_context_t vctx;
		vctx.push_back(hRC);
		video_contexts.push_back(vctx);
	    return 1 + i; // add 1 to reserve 0 = unassigned
	}
	else {
		video_contexts[iempty].push_back(hRC);
		return 1 + iempty;
	}
}

void
removeRenderingContextFromVideoContext(HGLRC hRC)
{
    video_contexts_t::iterator it = video_contexts.begin();
    for ( ; it != video_contexts.end(); it ++) 
    {
        video_context_t & vctx = *it;
		//if (vctx.size() > 1) {
			for (video_context_t::iterator irc = vctx.begin(); irc != vctx.end(); irc ++)
				if (*irc == hRC) {
					vctx.erase(irc);
					return;
				}
		//}
    }

    // Failed to remove the rendering context - something went really wrong
    assert(false);
}

//--- PUBLIC ROUTINES ---------------------------------------------------------

void
initGlew()
{
    static const char * DUMMY_WINDOW_CLASS_NAME = "GSM OPENGL INVISIBLE WINDOW CLASS";

    static bool init_done = false;

    if (! init_done)
    {
        // Register dummy window class
        WNDCLASS wc;
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = DefWindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = NULL;
        wc.hIcon = NULL;
        wc.hCursor = NULL;
        wc.hbrBackground = NULL;
        wc.lpszMenuName = NULL; 
        wc.lpszClassName = DUMMY_WINDOW_CLASS_NAME;
        if (!RegisterClass(&wc))
            throw EMSWinError(GetLastError(), "RegisterClass");

        // Create dummy (invisible) window
        HWND hWnd = CreateWindowEx(0, DUMMY_WINDOW_CLASS_NAME, "GL startup context window",
            WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZE, 
            0, 0, 1, 1,
            NULL, NULL, NULL /* module_instance() */, NULL);

        // Create initial GL context
        HDC hDC = GetDC(hWnd);
        if (hDC == NULL) throw EMSWinError(GetLastError(), "GetDC() of invisible OpenGL window" );
        if (! SetPixelFormat(hDC, selectPreliminaryPixelFormat(hDC), NULL) )
            throw EMSWinError(GetLastError(), "SetPixelFormat() on invisible OpenGL window");
        HGLRC hRC = wglCreateContext(hDC);
        if (hRC == 0) throw EMSWinError(GetLastError(), "wglCreateContext() for invisible OpenGL window");
        if (! wglMakeCurrent(hDC, hRC)) throw EMSWinError(GetLastError(), "wglMakeCurrent() failed on invisible OpenGL window");

        // Initialize GLEW
        glewExperimental = TRUE;

        if (glewInit() != GLEW_OK)
            throw EMSWinError(GetLastError(), "glewInit()");

        init_done = true;
    }
}

void
setupWindowForOpenGL(MSWinSurface *surf, ISurface::Attributes attribs)
{
    initGlew();

    // Get a DC
    HDC hDC = GetDC(surf->windowHandle());  // TODO: is it ok to call GetDC() every time a DC is needed?

    // Select and set a pixel format
    int pixel_format = selectPixelFormat(hDC, !attribs.test(ISurface::SINGLE_BUFFERED));
    if (! SetPixelFormat(hDC, pixel_format, NULL))
        throw EMSWinError(GetLastError(), "SetPixelFormat");

    // Create an OpenGL context
    HGLRC hRC = wglCreateContext(hDC);
    if (hRC == 0) throw EMSWinError(GetLastError(), "wglCreateContext");
    surf->setOpenGLContext(hRC);

    // Try to make the Rendering Context part of a Video Context
    surf->setVideoContextID( assignToVideoContext(hRC) );

#ifdef NOT_DEFINED
    // Initialize GLEW for this context
    glewExperimental = TRUE;
    if (glewInit() != GLEW_OK)
        throw EMSWinError(GetLastError(), "glewInit()");
#endif
}

void
retireRenderingContext(HGLRC hRC)
{
    removeRenderingContextFromVideoContext(hRC);
    if (! wglDeleteContext(hRC)) throw EMSWinError(GetLastError(), "wglDeleteContext");
}

} // ns gsm
