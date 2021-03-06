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

#include "mswin.hpp"

#include "../../isurface.hpp"

namespace gsm {

class MSWinSurface;

void
initOpenGL();

void
setupWindowForOpenGL(MSWinSurface *surf, ISurface::Attributes attribs);

void
removeRenderingContextFromVideoContext(HGLRC hRC);

void
retireRenderingContext(HGLRC hRC);

} // ns gsm

#endif // __GSM_MSWINOGL_HPP
