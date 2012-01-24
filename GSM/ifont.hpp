#ifndef __GSM_IFONT_HPP
#define __GSM_IFONT_HPP

#include <string>
#include <vector>
#include "types.hpp"
#include "gfxtypes.hpp"
#include "util/bitset.hpp"
#include "charset.hpp"

namespace gsm {

    class IBitmap;

    /** The IFont interface represents a rasterized font as provided by the Font 
        Provider.
     */
    class IFont {
    public:

        static const unsigned DEFAULT_MAX_BITMAP_SIZE = 1024;

        enum Type { ANY = 0, SYSTEM, DECORATIVE, MODERN, ROMAN, SCRIPT, SWISS };

        enum Attribute { BOLD, ITALIC, UNDERLINE, STRIKEOUT };
        typedef bitset<Attribute> Attributes;

        enum Alignment { LEFT, RIGHT, CENTERED };

        /** The BoundingBox indicates the rectangle where a given Character is
            located within a Bitmap (relative to its reference point).
            The GlyphMetrics structure just adds the advance width and heights
            (for western fonts, only the advance width is used).
         */
        struct GlyphMetrics: public BoundingBox {
            size_t adv_w;       /// advance width
            size_t adv_h;       /// advance height
        };

        struct GlyphBox: public GlyphMetrics {
            int xLeft, yTop;   /// Top-right corner of glyph rectangle
        };

        /** A Rasterization is a bitmap containing rasterized glyphs for a subset
            of a Character Set.
         */
        struct Rasterization {
            IBitmap                 *bitmap;            // 256 levels of gray
            CharacterSet            character_set;      // characters on bitmap
            std::vector<GlyphBox>   glyph_boxes;        // One per character in set
            Rasterization(IBitmap *bmp): bitmap(bmp) {}
        };

        enum RasterizeOption { POWER_OF_TWO };
        typedef bitset<RasterizeOption> RasterizeOptions;

        /** This function must be called repeatedly until the Character Set iterator
            has arrived at the end of the set.
            max_edge specifieds the maximum edge size of the generated Bitmap.
            The Bitmap returned by this call (via the Rasterization structure) is 
            owned by the caller.
         */
        virtual const Rasterization rasterize(const CharacterSet & charset, CharacterSet::iterator & it, 
            unsigned max_edge = 0, const RasterizeOptions options = 0) = 0;

        virtual const Extents getTextExtents(const unicode_t *text, unsigned len) = 0;

    };

} // ns gsm

#endif // __GSM_IFONTPROXY_HPP
