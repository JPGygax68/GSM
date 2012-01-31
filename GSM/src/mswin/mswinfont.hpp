#ifndef __MWIN_FONT_HPP
#define __MWIN_FONT_HPP

#include <Windows.h>
#include "../../charset.hpp"
#include "../../ifont.hpp"

namespace gsm {

class MSWinFontProvider;

class MSWinFont: public IFont {
public:
    ~MSWinFont();

    virtual const Rasterization rasterize(const CharacterSet & charset, CharacterSet::iterator & it, 
        unsigned max_edge, const RasterizeOptions options);

    virtual void getGlyphMetrics(unicode_t ch, GlyphMetrics &gm);

    virtual const BoundingBox getTextBounds(const unicode_t *text, unsigned len);

    bool isTrueType() const { return (metrics.tmPitchAndFamily & TMPF_TRUETYPE) != 0; }

private:
    MSWinFont(MSWinFontProvider *prov, HGDIOBJ hfont);

    friend class MSWinFontProvider;

    MSWinFontProvider   *provider;
    CharacterSet        charset;
    HGDIOBJ             hfont;
    TEXTMETRIC          metrics;
    HBITMAP             hbmp;
    HDC                 hdc;
};

} // ns gsm

#endif // __MWIN_FONTPROV_HPP
