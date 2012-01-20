#include <GL/glew.h>
#include <GL/wglew.h>

#include "mswinerr.hpp"
#include "mswinsurf.hpp"
#include "mswinogl.hpp"

namespace gsm {

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
    if (double_buffered) {
        // TODO: make sample count configurable
        for (unsigned samples = 4; samples > 0; samples -= 2) {
            iAttributes[19] = samples;
            bStatus = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes,1, pixelFormats, &numFormats);
            if (bStatus == GL_TRUE && numFormats > 0)
                return pixelFormats[0];
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

//--- PUBLIC ROUTINES ---------------------------------------------------------

void
initGlew()
{
    static const char * DUMMY_WINDOW_CLASS_NAME = "GSM OPENGL INVISIBLE WINDOW CLASS";

    static bool init_done = false;

    if (! init_done) {
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
        if (glewInit() != GLEW_OK)
            throw EMSWinError(GetLastError(), "glewInit()");

        init_done = true;
    }
}

void
setupWindowForOpenGL(MSWinSurface *surf, ISurface::Attributes attribs)
{
    initGlew();

    HDC hDC = GetDC(surf->windowHandle());  // TODO: is it ok to call GetDC() every time a DC is needed?

    // Select and set a pixel format
    int pixel_format = selectPixelFormat(hDC, !attribs.test(ISurface::SINGLE_BUFFERED));
    if (! SetPixelFormat(hDC, pixel_format, NULL))
        throw EMSWinError(GetLastError(), "SetPixelFormat");

    // Create an OpenGL context
    HGLRC hRC = wglCreateContext(hDC);
    if (hRC == 0) throw EMSWinError(GetLastError(), "wglCreateContext");
    surf->setOpenGLContext(hRC);
}

} // ns gsm
