#ifndef __GSM_MSWINOGL_HPP
#define __GSM_MSWINOGL_HPP

#include <Windows.h>
#include "../../isurface.hpp"

namespace gsm {

class MSWinSurface;

void
initGlew();

void
setupWindowForOpenGL(MSWinSurface *surf, ISurface::Attributes attribs);

void
retireContext(HGLRC hRC);

} // ns gsm

#endif // __GSM_MSWINOGL_HPP
