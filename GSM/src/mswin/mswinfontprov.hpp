#ifndef __MWIN_FONTPROV_HPP
#define __MWIN_FONTPROV_HPP

#include "../../compmgr.hpp"
#include "../../ifontprov.hpp"

namespace gsm {

class MSWinFontProvider: public IFontProvider {
public:
    static void
    registerIt() { registerComponent("FontProvider", new MSWinFontProvider() ); }

    virtual IFont * getFont(IFont::Type type, const std::string &name, size_t height, IFont::Attributes attribs);
};

} // ns gsm

#endif // __MWIN_FONTPROV_HPP
