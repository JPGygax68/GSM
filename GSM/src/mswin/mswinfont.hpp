/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __MWIN_FONT_HPP
#define __MWIN_FONT_HPP

#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "../../charlist.hpp"
#include "../../ifontprov.hpp"
#include "../../ifont.hpp"

namespace gsm {

class MSWinFontProvider;

class MSWinFont: public IFont {
public:
    ~MSWinFont();

	virtual const CharacterList *
	characterList();

    virtual const Rasterization 
	rasterize(const CharacterList & charlist_, CharacterList::iterator & it, unsigned max_edge, const RasterizeOptions options);

	virtual const FontHeight
	getFontHeight();

    virtual void 
	getGlyphMetrics(unicode_t ch, GlyphMetrics &gm);

    virtual const BoundingBox 
	getTextBounds(const unicode_t *text, unsigned len);

    bool 
	isTrueType() const { return (metrics.tmPitchAndFamily & TMPF_TRUETYPE) != 0; }

private:
    MSWinFont(MSWinFontProvider *prov, Type type, const std::string & name, unsigned height, Attributes attribs, CharSet charset = CHARSET_DEFAULT);

    friend class MSWinFontProvider;

    MSWinFontProvider   *provider;
    CharacterList       charlist;
    HGDIOBJ             hfont;
    TEXTMETRIC          metrics;
    HBITMAP             hbmp;
    HDC                 hdc;
};

} // ns gsm

#endif // __MWIN_FONTPROV_HPP
