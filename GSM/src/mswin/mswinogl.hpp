#ifndef __GSM_MSWINOGL_HPP
#define __GSM_MSWINOGL_HPP

namespace gsm {

class MSWinSurface;

void
initGlew();

void
setupWindowForOpenGL(MSWinSurface *surf, ISurface::Attributes attribs);

} // ns gsm

#endif // __GSM_MSWINOGL_HPP
