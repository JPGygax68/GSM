#include <algorithm>
#include "../../ifont.hpp"
#include "../../util/text.hpp"

namespace gsm {

void
traverseText(IFont *font, const unicode_t *text, size_t len, int linespacing, ttCallbackFunc cb, void *userdata)
{
    IFont::GlyphMetrics gm1, gm2;
    font->getGlyphMetrics('M', gm1);
    font->getGlyphMetrics('g', gm2);
    BoundingBox gmBox = static_cast<BoundingBox&>(gm1) | static_cast<BoundingBox&>(gm2);

    unicode_t prevch = 0;
    int x = 0, y = 0;

    for (unsigned i = 0; len == 0 && text[i] != 0 || len > 0 && i < len; i ++) 
    {
        unicode_t ch = text[i];
        // Control character ?
        if (ch == 13 || ch == 10) 
        {
            // Line break ?
            if ((ch == 13 && prevch != 10) || (ch == 10 && prevch != 13)) {
                int dy = std::max(-gmBox.yMin, gmBox.yMax) + linespacing;
                y += dy;
                x = 0;
            }
        }
        else {
            // Get glyph measurements
            IFont::GlyphMetrics gm;
            font->getGlyphMetrics(ch, gm);
            // Call the callback function
            (*cb)(userdata, i, static_cast<BoundingBox&>(gm), x, y, gm.adv_w, gm.adv_h);
            // Advance
            x += (signed) gm.adv_w;
        }
    }
}

const BoundingBox
getTextBounds(IFont *font, const unicode_t *text, size_t len, int linespacing)
{
    struct CBS {    // Callback Struct
        BoundingBox bbox;
        static void
        callback(void *userdata, unsigned char_idx, const BoundingBox &chbox, int x, int y, int dx, int dy) {
            CBS *that = static_cast<CBS*>(userdata);
            that->bbox.merge(chbox, x, y);
        }
    } cbs;

    traverseText(font, text, len, linespacing, CBS::callback, &cbs);

    return cbs.bbox;
}

} // ns gsm
