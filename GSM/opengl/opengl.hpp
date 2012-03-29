/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_OPENGL_OPENGL_HPP
#define __GSM_OPENGL_OPENGL_HPP

#include <GL/glew.h>

#include "../dll.h"
#include "../types.hpp"
#include "../ifont.hpp"

namespace gsm {

class ISurface;

namespace ogl {

typedef void * fonthandle_t;

//--- UTILITIES --------------------------------------------------------------

/** Draw a simple rectangle.
 */
void GSM_API
rectangle(unsigned wb, unsigned hb, int x = 0, int y = 0);

/** Texture must already be bound when calling this method.
    wb, hb:		bitmap width and height
    xr, yr:		origin from top-left of rectangle within texture bitmap (in pixels)
    wr, hr:		width and height of rectangle within texture bitmap (in pixels)
    x, y  :     position of top-left corner on screen
	yFlipped:	if true, the bitmap is "upside-down" (bottom pixels first in memory)
    */
void GSM_API
texturedRectangle(unsigned wb, unsigned hb, int xr, int yr, unsigned wr, unsigned hr, int x = 0, int y = 0, 
	bool yFlipped = false);

//--- TEXT & FONTS ------------------------------------------------------------

/** Prepares a Font for use in the specified Video Context.
    The function returns a handle to be specified in text rendering calls.
    TODO: distinguish between monitors to take advantage of ClearType
 */
fonthandle_t GSM_API
prepareFont(IFont *font, int vidCtxID);

/** Saves the current projection matrix and sets up a new projection so that 2D coordinates 
    match directly with the pixel grid, with the top-left corner being at (0, 0) and the 
    bottom-right corner at (width, height).
    DOES NOT clear the MODELVIEW matrix!
 */
void GSM_API
prepareForTextRendering();

/** Restores the previous projection matrix. This is equivalent to doing glPopMatrix()
    with the GL_PROJECTION matrix active, then selecting the GL_MODELVIEW matrix.
 */
void GSM_API
doneWithTextRendering();

/** Renders Unicode text, which may contain line breaks (CR), but no other control
    characters.
 */
void GSM_API
renderText(fonthandle_t fonthandle, const unicode_t *text, int &dx, int &dy);

/** Use this version if you do not need to track output position advances.
 */
void GSM_API
renderText(fonthandle_t fonthandle, const unicode_t *text);

/** Determine the width of a line of text.
 */
void GSM_API
measureText(fonthandle_t fonthandle, const unicode_t *text, size_t len, int interline, BoundingBox &bbox);

/** Render text, aligning it.
    This routine does not attempt to break lines that do not fit the specified width.
 */
void GSM_API
renderTextAligned(fonthandle_t fonthandle, const unicode_t *text, size_t len, IFont::Alignment align, int interline, unsigned w, int &y);

int GSM_API
calcLeading(fonthandle_t font1, fonthandle_t font2 = NULL, unsigned lineSpacing = 0);

//--- DEBUG -------------------------------------------------------------------

bool  GSM_API
dbg_getFontTexture(fonthandle_t fonthandle, unsigned range_index, GLuint &tex, unsigned &w, unsigned &h);

} // ns opengl
} // ns gsm

#endif // __GSM_OPENGL_OPENGL_HPP
