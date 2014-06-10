/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_IFONT_HPP
#define __GSM_IFONT_HPP

#include <string>
#include <vector>

#include "dll.h"
#include "types.hpp"
#include "gfxtypes.hpp"
#include "util/bitset.hpp"
#include "charlist.hpp"

namespace gsm {

    class IBitmap;

    /** The IFont interface represents a rasterized font as provided by the Font 
        Provider.
     */
    class GSM_API IFont {
    public:

        static const unsigned DEFAULT_MAX_BITMAP_SIZE = 1024;

        enum Type { ANY = 0, SYSTEM, DECORATIVE, MODERN, ROMAN, SCRIPT, SWISS };

		/** The CharSet enumeration is currently a direct reflection of the MS Windows character set enumeration.
			This may change as support for other font subsystems is added.
		 */
		enum CharSet { CHARSET_DEFAULT = 0, ANSI, BALTIC, CHINESEBIG5, EASTEUROPE, GB2312, GREEK, HANGUL, MAC, OEM, RUSSIAN, SHIFTJIS, SYMBOL, TURKISH, VIETNAMESE };

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

		struct FontHeight {
			int ascent, descent; // pixels from the baseline, both positive
		};

        /** A Rasterization is a bitmap containing rasterized glyphs for a subset
            of a Character Set.
         */
        struct Rasterization {
            IBitmap                 *bitmap;            // 256 levels of gray
            CharacterList           character_list;      // characters on bitmap
            std::vector<GlyphBox>   glyph_boxes;        // One per character in set
        };

        enum RasterizeOption { POWER_OF_TWO };
        typedef bitset<RasterizeOption> RasterizeOptions;

		/** Returns list of all available characters (depends - most likely - on character set 
			specified at creation).
		 */
		virtual const CharacterList *
		characterList() = 0;

		virtual const FontHeight getFontHeight() = 0;

        /** This function must be called repeatedly until the Character Set iterator
            has arrived at the end of the set.
            max_edge specifies the maximum edge size of the generated Bitmap.
            The Bitmap returned by this call (via the Rasterization structure) is 
            owned by the caller.
         */
        virtual const Rasterization
		rasterize(const CharacterList & charlist, CharacterList::iterator & it, unsigned max_edge = 0, const RasterizeOptions options = 0) = 0;

        virtual void getGlyphMetrics(unicode_t ch, GlyphMetrics &gm) = 0;

        virtual const BoundingBox getTextBounds(const unicode_t *text, unsigned len) = 0;

    };

} // ns gsm

#endif // __GSM_IFONT_HPP
