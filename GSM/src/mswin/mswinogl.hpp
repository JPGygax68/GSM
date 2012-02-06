/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

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
