#ifndef __GSM_OPENGL_OPENGL_HPP
#define __GSM_OPENGL_OPENGL_HPP

#include "../types.hpp"
#include "../ifont.hpp"

namespace gsm {

namespace ogl {

/** Prepares a Font for use in the current OpenGL context.
    The function returns a handle to be specified in text rendering calls.
 */
int prepareFont(IFont *font, int resourcePoolID);

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
    The x and y parameter, upon call, specify the starting point, with y specifying
    the baseline.
    Upon return, x and y contain the point where the next character would be rendered.
 */
void renderText(int fonthandle, int &x, int &y, const unicode_t *text);

/** Determine the width and height needed by unicode text string, which can
    contain newlines.
    The dimensions returned include line spacing.
 */
void measureText(int fonthandle, const unicode_t *text, unsigned &w, unsigned &h);

/** Render text, aligning it.
    This routine does not attempt to break lines that do not fit the specified width.
 */
void renderTextAligned(int fonthandle, int &x, int &y, unsigned w, const unicode_t *text, IFont::Alignment align);

} // ns opengl
} // ns gsm

#endif // __GSM_OPENGL_OPENGL_HPP