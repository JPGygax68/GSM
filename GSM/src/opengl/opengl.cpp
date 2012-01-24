#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/GL.h>
#include <map>
#include "../../util/oglhelper.hpp"
#include "../../ibitmap.hpp"
#include "../../opengl/opengl.hpp"

namespace gsm {

namespace ogl {

//--- PRIVATE TYPES -----------------------------------------------------------

struct FontBinding {
    CharacterSet char_set;
    std::vector<int> list_bases;    // one Display List base per Character Range
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

/** Texture must already be bound when calling this method.
    wb, hb:		bitmap width and height
    xr, yr:		top-left origin of rectangle within texture bitmap (in pixels)
    wr, hr:		width and height of rectangle within texture bitmap (in pixels)
    x, y  :     position of top-left corner on screen
    */
static void
texturedRectangle(unsigned wb, unsigned hb, int xr, int yr, unsigned wr, unsigned hr, int x, int y)
{
	//GLdouble wtex = pixel_to_texture_pos(wb, 1);
	//GLdouble htex = pixel_to_texture_pos(hb, 1);
	GLdouble x1t = pixelToTexturePos(wb, xr);
	GLdouble y1t = pixelToTexturePos(hb, yr);
	GLdouble x2t = pixelToTexturePos(wb, xr + wr);
	GLdouble y2t = pixelToTexturePos(hb, yr + hr);
	OGL(glBegin, (GL_QUADS) );
		glTexCoord2d(x1t, y1t); glVertex2i(   x, y   );
		glTexCoord2d(x1t, y2t); glVertex2i(   x, y+hr);
		glTexCoord2d(x2t, y2t); glVertex2i(x+wr, y+hr);
		glTexCoord2d(x2t, y1t); glVertex2i(x+wr, y   );
	OGLI(glEnd, ());
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

    FontBinding *binding = new FontBinding();

    for (CharacterSet::iterator it = charset.begin(); it != charset.end(); it ++) {
        // Rasterize
        IFont::Rasterization rast = font->rasterize(charset, it);
        // Some preparation
        IBitmap *bmp = rast.bitmap;
        binding->list_bases.resize(rast.character_set.ranges().size());
        // Create texture from rasterized bitmap
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
            binding->list_bases[ir] = glGenLists(range.num_chars);
            // Create a Display List for each character in the range
            for (unsigned ic = 0; ic < range.num_chars; ic ++) {
                OGL(glBindTexture, (GL_TEXTURE_2D, texture) );
                IFont::GlyphBox &gb = rast.glyph_boxes[ir];
                GLuint list = binding->list_bases[ir] + ic;
                OGL(glNewList, (list, GL_COMPILE));
                // Leave gap to the left, adjust for ascender
                OGL(glPushMatrix, ());
                // Render the bmpglyph (into the Display List)
                if (gb.width() > 0 && gb.height() > 0) {
                    int xtex = gb.xLeft, ytex = gb.yTop;
                    unsigned wtex = gb.width(), htex = gb.height();
                    texturedRectangle(bmp->width(), bmp->height(), xtex, ytex, wtex, htex, gb.xMin, gb.yMin);
                }
                OGL(glPopMatrix, ());
                // Advance to start position of next character
                OGL(glTranslatef, ((GLfloat)gb.adv_w, (GLfloat)gb.adv_h, 0) );
                //glRasterPos2i(0, 0); // update raster pos
                OGL(glEndList, ());
            }
            // Add range to binding's character set
            binding->char_set.add(range);
        }
        // Done with this bitmap
        delete rast.bitmap;
        rast.bitmap = NULL;
    }
}

//--- PUBLIC ROUTINE IMPLEMENTATIONS ------------------------------------------

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
    FontBinding *binding = bindFont(font, NULL);
    fonts->insert( fonts_t::value_type(font, binding) );

    return binding;
}

void releaseFont(IFont *font, int vidCtxID)
{
}

} // ns ogl

} // ns gsm
