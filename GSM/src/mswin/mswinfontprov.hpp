#ifndef __MWIN_FONTPROV_HPP
#define __MWIN_FONTPROV_HPP

#include "../../ifontprov.hpp"

namespace gsm {

class MSWinFontProvider: public IFontProvider {
public:
    virtual IFont * getFont(IFont::Type type, const std::string &family, size_t height, IFont::Attributes attribs);
};

} // ns gsm

#endif // __MWIN_FONTPROV_HPP
