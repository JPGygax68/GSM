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

    virtual const Rasterization rasterize(CharacterSet & charset, CharacterSet::Iterator & it, 
        unsigned max_edge, const RasterizeOptions options);

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
