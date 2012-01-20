#include <Windows.h>
#include "mswinfont.hpp"
#include "mswinfontprov.hpp"

namespace gsm {

IFont *
MSWinFontProvider::getFont(IFont::Type type, const std::string &name, size_t height, IFont::Attributes attribs)
{
    HGDIOBJ hfont = 0;

    if (name.empty() && type == IFont::SYSTEM) {
        hfont = GetStockObject(ANSI_VAR_FONT);
    }
    else {
        //if (height == 0) height = 16; // TODO: use constant
        LOGFONTA lf;
        lf.lfHeight = height;
        lf.lfWidth = 0;
        lf.lfEscapement = 0;
        lf.lfOrientation = 0;
        lf.lfWeight = attribs.test(IFont::BOLD) ? FW_BOLD : FW_NORMAL;
        lf.lfItalic = attribs.test(IFont::ITALIC) ? 1 : 0;
        lf.lfUnderline = attribs.test(IFont::UNDERLINE) ? TRUE : FALSE;
        lf.lfStrikeOut = attribs.test(IFont::STRIKEOUT) ? TRUE : FALSE;
        lf.lfCharSet = ANSI_CHARSET;
        lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
        lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        lf.lfQuality = height <= 18 ? // TODO: use constant
            ANTIALIASED_QUALITY : PROOF_QUALITY;
        lf.lfQuality = PROOF_QUALITY; //height <= 18 ? // TODO: use constant
        switch (type) {
            case IFont::DECORATIVE:	lf.lfPitchAndFamily = FF_DECORATIVE; break;
            case IFont::MODERN:		lf.lfPitchAndFamily = FF_MODERN; break;
            case IFont::ROMAN:		lf.lfPitchAndFamily = FF_ROMAN; break;
            case IFont::SCRIPT:		lf.lfPitchAndFamily = FF_SCRIPT; break;
            case IFont::SWISS:		lf.lfPitchAndFamily = FF_SWISS; break;
            default:				lf.lfPitchAndFamily = FF_DONTCARE; break;
        }
        strncpy_s(lf.lfFaceName, LF_FACESIZE, name.c_str(), LF_FACESIZE);
        hfont = CreateFontIndirectA(&lf);
    }

    return new MSWinFont(this, hfont);
}

} // ns gsm
