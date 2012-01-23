#include <map>
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
        // Create texture from rasterized bitmap
        // TODO
        // Add range by range
        CharacterSet::ranges_t & ranges = rast.character_set.ranges();
        for (CharacterSet::ranges_t::iterator i2 = ranges.begin(); i2 != ranges.end(); i2++) {
            CharacterSet::Range & range = *i2;
            // Allocate sequence of Display List IDs for the range
            // TODO
            // Create a Display List for each character in the range
            // TODO
            // Add range to binding's character set
            binding->char_set.add(range);
        }
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
