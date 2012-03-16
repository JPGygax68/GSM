/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/GLew.h>
#include <map>
#include <cmath>
#include "../../ibitmap.hpp"
#include "../../gfxtypes.hpp"
#include "../../opengl/opengl.hpp"

namespace gsm {

namespace ogl {

//--- PRIVATE TYPES -----------------------------------------------------------

struct FontBinding {
    IFont                   *font;
    unsigned                ascent, descent;    // ascend and descent
    CharacterSet            char_set;
    std::vector<int>        list_bases;         // one Display List base per Character Range (inside char_set)
    std::vector<GLuint>     textures;
    std::vector<Extents>    tex_sizes;
};

typedef std::map<IFont*, FontBinding*> fonts_t;

typedef std::map<int, fonts_t*> contextmap_t;

//--- PRIVATE DATA ------------------------------------------------------------

contextmap_t contexts; // one per Video Context

//--- PRIVATE ROUTINES --------------------------------------------------------

static GLdouble
pixelToTexturePos(unsigned size, int pos) 
{ 
    return ((GLdouble)pos / (GLdouble)size);
}

static fonts_t *
getFontList(int vidCtxID)
{
    // Look for Font List for this Video Context
    contextmap_t::iterator it = contexts.find(vidCtxID);
    if (it != contexts.end()) return it->second;

    // Not found, so create it and register it with the Video Context
    fonts_t *fonts = new fonts_t();
    contexts.insert( contextmap_t::value_type(vidCtxID, fonts) );

    return fonts;
}

static FontBinding *
bindFont(IFont *font, const CharacterSet *charset_)
{
    const CharacterSet & charset = charset_ ? *charset_ : CharacterSet::LATIN1();

    FontBinding *bind = new FontBinding();
    bind->font = font;

    // Measure ascent and descent
    // TODO: make dependent on character set ?
    // TODO: move to Font/Font Provider ?
    bind->ascent = bind->descent = 0;
    for (const unicode_t *p = L"gM"; *p != 0; p++) {
        IFont::GlyphMetrics gm;
        font->getGlyphMetrics(*p, gm);
        // Update ascent & descent
        if ((-gm.yMin) > (signed)bind->ascent) bind->ascent = (unsigned) (-gm.yMin);
        if (gm.yMax > (signed)bind->descent) bind->descent = (unsigned) (gm.yMax);
    }

    // Convert each character in the character set into a Display List
    for (CharacterSet::iterator it = charset.begin(); it != charset.end(); )
    {
        // Rasterize
        IFont::Rasterization rast = font->rasterize(charset, it);
        // Some preparation
        IBitmap *bmp = rast.bitmap;
        bind->list_bases.reserve(rast.character_set.ranges().size());
        bind->textures.reserve(rast.character_set.ranges().size());
        bind->tex_sizes.reserve(rast.character_set.ranges().size());
        // Create alpha texture from rasterized bitmap
        GLuint texture;
        OGL(glGenTextures, (1, &texture) );
        OGL(glBindTexture, (GL_TEXTURE_2D, texture) );
        OGL(glTexParameteri, (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
        OGL(glTexParameteri, (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
        OGL(glTexImage2D, (GL_TEXTURE_2D, 0, GL_ALPHA, bmp->width(), bmp->height(), 0, GL_ALPHA, GL_UNSIGNED_BYTE, bmp->data()) );
        OGL(glBindTexture, (GL_TEXTURE_2D, 0) );
        // TODO: delete bitmap
        // Add range by range
        CharacterSet::ranges_t & ranges = rast.character_set.ranges();
        unsigned ig = 0; // glyph index
        for (unsigned ir = 0; ir < ranges.size(); ir++) 
        {
            CharacterSet::Range & range = ranges[ir];
            // Allocate sequence of Display List IDs for the range
            GLuint lists_base = glGenLists(range.num_chars);
            // Create a Display List for each character in the range
            for (unsigned ic = 0; ic < range.num_chars; ic ++, ig++) {
                OGL(glBindTexture, (GL_TEXTURE_2D, texture) );
                IFont::GlyphBox &gb = rast.glyph_boxes[ig];
                GLuint list = lists_base + ic;
                OGL(glNewList, (list, GL_COMPILE));
                OGL(glBindTexture, (GL_TEXTURE_2D, texture));
                // Render the bmpglyph (into the Display List)
                if (gb.width() > 0 && gb.height() > 0) {
                    int xtex = gb.xLeft, ytex = gb.yTop;
                    unsigned wtex = gb.width(), htex = gb.height();
                    texturedRectangle(bmp->width(), bmp->height(), xtex, ytex, wtex, htex, gb.xMin, gb.yMin);
                }
                OGL(glBindTexture, (GL_TEXTURE_2D, 0));
                // Advance to start position of next character
                OGL(glTranslatef, ((GLfloat)gb.adv_w, (GLfloat)gb.adv_h, 0) );
                //glRasterPos2i(0, 0); // update raster pos
                OGL(glEndList, ());
            }
            // Add range to bind's character set (+debug info)
            bind->char_set.add(range);
            bind->list_bases.push_back(lists_base);
            bind->textures.push_back(texture);
            bind->tex_sizes.push_back(Extents(bmp->width(), bmp->height()));
        }
        // Done with this bitmap
        delete rast.bitmap;
        rast.bitmap = NULL;
    }

    return bind;
}

static void
traverseText(fonthandle_t fonthandle, const unicode_t *text, size_t len, int interline, BoundingBox &bbox, int &x, int &y, bool measure, bool draw)
{
    FontBinding &bind = *static_cast<FontBinding*>(fonthandle);

    int dx = 0, dy = 0;
    bbox.xMin = bbox.yMin = INT_MAX;
    bbox.xMax = bbox.yMax = INT_MIN;
    unicode_t prevch = 0;

    for (unsigned i = 0; len == 0 && text[i] != 0 || len > 0 && i < len; i ++) 
    {
        unicode_t ch = text[i];
        // Control character ?
        if (ch == 13 || ch == 10) 
        {
            if ((ch == 13 && prevch != 10) || (ch == 10 && prevch != 13)) {
                int ddx = -dx;
                int ddy = max((signed)bind.ascent + interline, (signed)bind.descent + interline);
                if (draw) glTranslatef((GLfloat)ddx, (GLfloat)ddy, 0);
                dy += ddy;
                dx = 0;
            }
        }
        else {
            // Get glyph measurements
            IFont::GlyphMetrics gm;
            bind.font->getGlyphMetrics(ch, gm);
            // Update text field bounding box (if asked for)
            if (measure) {
                int xmin = dx + gm.xMin;
                int xmax = dx + gm.xMax;
                if (xmin < bbox.xMin) bbox.xMin = xmin;
                if (xmax > bbox.xMax) bbox.xMax = xmax;
                int ymin = dy + gm.yMin;
                int ymax = dy + gm.yMax;
                if (ymin < bbox.yMin) bbox.yMin = ymin;
                if (ymax > bbox.yMax) bbox.yMax = ymax;
            }
            // Draw (if asked for)
            if (draw) {
                // Find the character within the bound font
                CharacterSet &cs = bind.char_set;
                // Traverse all ranges
                for (unsigned ir = 0; ir < cs.ranges().size(); ir ++) {
                    CharacterSet::Range &rn = cs.ranges()[ir];
                    // Character is within this Range ?
                    if (ch >= rn.first && ch < (rn.first + rn.num_chars)) {
                        // Compute Display List ID
                        GLuint dl = bind.list_bases[ir] + (ch - rn.first);
                        // Play the Display List
                        OGL(glCallList, (dl));
                    }
                }
            }
            // Advance
            dx += (signed) gm.adv_w;
        }
    }

    x += dx;
    y += dy;
}

//--- PUBLIC ROUTINE IMPLEMENTATIONS ------------------------------------------

void
rectangle(unsigned wb, unsigned hb, int x , int y)
{
    glDisable(GL_TEXTURE_2D);
    OGL(glBegin, (GL_QUADS) );
        OGL(glVertex2i, (   x, y   ));
        OGL(glVertex2i, (   x, y+hb));
        OGL(glVertex2i, (x+wb, y+hb));
        OGL(glVertex2i, (x+wb, y   ));
    OGLI(glEnd, ());
    glEnable(GL_TEXTURE_2D);
}

void
texturedRectangle(unsigned wb, unsigned hb, int xr, int yr, unsigned wr, unsigned hr, int x, int y)
{
    GLdouble x1t = pixelToTexturePos(wb, xr);
    GLdouble y1t = 1 - pixelToTexturePos(hb, yr);
    GLdouble x2t = pixelToTexturePos(wb, xr + wr);
    GLdouble y2t = 1 - pixelToTexturePos(hb, yr + hr);
    OGL(glBegin, (GL_QUADS) );
        OGL(glTexCoord2d, (x1t, y1t)); OGL(glVertex2i, (   x, y   ));
        OGL(glTexCoord2d, (x1t, y2t)); OGL(glVertex2i, (   x, y+hr));
        OGL(glTexCoord2d, (x2t, y2t)); OGL(glVertex2i, (x+wr, y+hr));
        OGL(glTexCoord2d, (x2t, y1t)); OGL(glVertex2i, (x+wr, y   ));
    OGLI(glEnd, ());
}

void
drawBevelFrame(unsigned w, unsigned h, unsigned bw, const Float4 *colors, int x, int y)
{
    // Draw north slope
    glColor4fv(colors[0]);
    OGL(glBegin, (GL_TRIANGLE_STRIP));
        glVertex2i(x, y);
        glVertex2i(x + bw, y + bw);
        glVertex2i(x + w, y );
        glVertex2i(x + w - bw, y + bw);
    OGLI(glEnd, ());
    // East slope
    glColor4fv(colors[1]);
    OGL(glBegin, (GL_TRIANGLE_STRIP));
        glVertex2i(x + w, y);
        glVertex2i(x + w - bw, y + bw);
        glVertex2i(x + w, y + h);
        glVertex2i(x + w - bw, y + h - bw);
    OGLI(glEnd, ());
    // South slope
    glColor4fv(colors[2]);
    OGL(glBegin, (GL_TRIANGLE_STRIP));
        glVertex2i(x + w - bw, y + h - bw);
        glVertex2i(x + bw, y + h - bw);
        glVertex2i(x + w, y + h);
        glVertex2i(x, y + h);
    OGLI(glEnd, ());
    // Left slope
    glColor4fv(colors[3]);
    OGL(glBegin, (GL_TRIANGLE_STRIP));
        glVertex2i(x, y);
        glVertex2i(x, y + h);
        glVertex2i(x + bw, y + bw);
        glVertex2i(x + bw, y + h - bw);
    OGLI(glEnd, ());
}

// TODO: Character Set

fonthandle_t
prepareFont(IFont *font, int vidCtxID)
{
    // Get the Font List for the specified Video Context
    fonts_t *fonts = getFontList(vidCtxID);

    // Check if the Font has already been bound
    fonts_t::iterator it = fonts->find(font);
    if (it != fonts->end()) return it->second;

    // No, so we need to "bind" the Font now, and enter it into the list
    FontBinding *bind = bindFont(font, NULL);
    fonts->insert( fonts_t::value_type(font, bind) );

    return bind;
}


void
prepareForTextRendering()
{
    OGL(glPushAttrib, (GL_COLOR_BUFFER_BIT|GL_ENABLE_BIT));
    OGL(glDisable, (GL_LIGHTING));
    OGL(glEnable, (GL_TEXTURE_2D));
    OGL(glTexEnvi, (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));
    OGL(glEnable, (GL_BLEND));
    OGL(glDisable, (GL_DEPTH_TEST));
    OGL(glEnable, (GL_CULL_FACE));
    OGL(glBlendFunc, (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

    OGL(glMatrixMode, (GL_PROJECTION));
    OGL(glPushMatrix, ());
    OGL(glLoadIdentity, ());
    GLint vp[4];
    OGL(glGetIntegerv, (GL_VIEWPORT, vp));
    OGL(glOrtho, (0, vp[2], vp[3], 0, -1, 1));

    OGL(glMatrixMode, (GL_MODELVIEW));
    OGL(glPushMatrix, ());
    OGL(glLoadIdentity, ());
}

void
doneWithTextRendering()
{
    OGL(glMatrixMode, (GL_PROJECTION));
    OGL(glPopMatrix, ());
    OGL(glMatrixMode, (GL_MODELVIEW));
    OGL(glPopMatrix, ());
    OGL(glPopAttrib, ());
}

void
releaseFont(IFont *font, int vidCtxID)
{
    // TODO
}

void
renderText(fonthandle_t fonthandle, const unicode_t *text, int &x, int &y)
{
    BoundingBox bbox;
    traverseText(fonthandle, text, 0, 0, bbox, x, y, false, true);
}

void
renderText(fonthandle_t fonthandle, const unicode_t *text)
{
    int dx = 0, dy = 0;
    BoundingBox bbox;
    traverseText(fonthandle, text, 0, 0, bbox, dx, dy, false, true);
    glTranslatef((GLfloat) -dx, (GLfloat) -dy, 0);
}

void
measureText(fonthandle_t fonthandle, const unicode_t *text, size_t len, int interline, BoundingBox &bbox)
{
    int dx = 0, dy = 0;
    traverseText(fonthandle, text, len, interline, bbox, dx, dy, true, false);
}

void
renderTextAligned(fonthandle_t fonthandle, const unicode_t *text, size_t len, IFont::Alignment align, int interline, unsigned w, int &y)
{
    FontBinding &bind = *static_cast<FontBinding*>(fonthandle);

    unicode_t prevch = 0;
    unsigned istart = 0;
    int x = 0, xmin = INT_MAX, xmax = INT_MIN;

    for (unsigned i = 0; ; i ++) 
    {
        // End of text ?
        bool eot = (len == 0 && text[i] == 0) || (len > 0 && i == len);

        // Line complete ?
        if (eot || (text[i] == 13 || text[i] == 10))
        {
            if (eot || ((text[i] == 13 && prevch != 10) || (text[i] == 10 && prevch != 13)))
            {
                // Alignment
                int dx;
                int wl = xmax - xmin;
                if (align == IFont::CENTERED)
                    dx = ((signed)w - wl) / 2;
                else if (align == IFont::RIGHT)
                    dx = (signed) w - wl;
                else
                    dx = 0;
                glTranslatef((GLfloat) dx, 0, 0);
                // Draw line of text
                int dumx = 0, dumy = 0;
                BoundingBox dumbb;
                traverseText(fonthandle, text + istart, i - istart, 0, dumbb, dumx, dumy, false, true);
                int dy = (int) bind.ascent + interline; // TODO: leading!
                // Carriage return + linefeed (so to speak)
                glTranslatef((GLfloat) - (dx + dumx), (GLfloat)dy, 0);
                y += dy;
                // Reset line width accumulation variables
                xmin = INT_MAX, xmax = INT_MIN;
                x = 0;
            }
            if (eot) break;
            istart = i + 1;
        }
        else {
            // Get glyph measurements
            IFont::GlyphMetrics gm;
            bind.font->getGlyphMetrics(text[i], gm);
            if ((x + gm.xMin) < xmin) xmin = x + gm.xMin;
            if ((x + gm.xMax) > xmax) xmax = x + gm.xMax;
            // Advance
            x += (signed) gm.adv_w;
        }
        prevch = text[i];
    }
}

// TODO: move implementation to non-OpenGL utility module
int
calcLeading(fonthandle_t fh1, fonthandle_t fh2, unsigned lineSpacing)
{
    FontBinding &bind1 = *static_cast<FontBinding*>(fh1);
    FontBinding &bind2 = *static_cast<FontBinding*>(fh2 != NULL ? fh2 : fh1);
    
    return bind1.descent + lineSpacing + bind2.ascent;
}

//--- DEBUG -------------------------------------------------------------------

bool
dbg_getFontTexture(fonthandle_t fonthandle, unsigned range_index, GLuint &tex, unsigned &w, unsigned &h)
{
    FontBinding &bind = *static_cast<FontBinding*>(fonthandle);

    if (range_index < bind.char_set.ranges().size()) {
        tex = bind.textures[range_index];
        w = bind.tex_sizes[range_index].w;
        h = bind.tex_sizes[range_index].h;
        return true;
    }

    return false;
}

} // ns ogl

} // ns gsm
