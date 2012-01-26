#ifndef __GSM_OPENGL_OPENGL_HPP
#define __GSM_OPENGL_OPENGL_HPP

#include "../types.hpp"
#include "../ifont.hpp"

namespace gsm {

class ISurface;

namespace ogl {

typedef void * fonthandle_t;

//--- UTILITIES --------------------------------------------------------------

/** Texture must already be bound when calling this method.
    wb, hb:		bitmap width and height
    xr, yr:		origin from top-left of rectangle within texture bitmap (in pixels)
    wr, hr:		width and height of rectangle within texture bitmap (in pixels)
    x, y  :     position of top-left corner on screen
    */
void
texturedRectangle(unsigned wb, unsigned hb, int xr, int yr, unsigned wr, unsigned hr, int x, int y);

//--- TEXT & FONTS ------------------------------------------------------------

/** Prepares a Font for use in the specified Video Context.
    The function returns a handle to be specified in text rendering calls.
    TODO: distinguish between monitors to take advantage of ClearType
 */
fonthandle_t prepareFont(IFont *font, int vidCtxID);

/** Saves the current projection matrix and sets up a new projection so that 2D coordinates 
    match directly with the pixel grid, with the top-left corner being at (0, 0) and the 
    bottom-right corner at (width, height).
 */
void pushDirectPixelMapping();

/** Restores the previous projection matrix. This is equivalent to doing glPopMatrix()
    with the GL_PROJECTION matrix active, then selecting the GL_MODELVIEW matrix.
 */
void popProjection();

/** Renders Unicode text, which may contain line breaks (CR), but no other control
    characters.
 */
void renderText(fonthandle_t fonthandle, const unicode_t *text, int &dx, int &dy);

/** Use this version if you do not need to track output position advances.
 */
void renderText(fonthandle_t fonthandle, const unicode_t *text);

/** Determine the width of a line of text.
 */
void measureText(fonthandle_t fonthandle, const unicode_t *text, size_t len, BoundingBox &bbox);

/** Render text, aligning it.
    This routine does not attempt to break lines that do not fit the specified width.
 */
void renderTextAligned(fonthandle_t fonthandle, const unicode_t *text, size_t len, IFont::Alignment align, int interline, unsigned w, int &y);

int calcLeading(fonthandle_t font1, fonthandle_t font2 = NULL, unsigned lineSpacing = 0);

//--- DEBUG -------------------------------------------------------------------

bool dbg_getFontTexture(fonthandle_t fonthandle, unsigned range_index, GLuint &tex, unsigned &w, unsigned &h);

} // ns opengl
} // ns gsm

#endif // __GSM_OPENGL_OPENGL_HPP
