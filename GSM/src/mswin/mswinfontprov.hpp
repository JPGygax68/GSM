/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __MWIN_FONTPROV_HPP
#define __MWIN_FONTPROV_HPP

#include "../../compmgr.hpp"
#include "../../ifontprov.hpp"

namespace gsm {

class MSWinFontProvider: public IFontProvider {
public:
    static void
    registerIt() { registerComponent("FontProvider", new MSWinFontProvider() ); }

    virtual IFont *
	getFont(IFont::Type type, const std::string &name, size_t height, IFont::Attributes attribs, IFont::CharSet = IFont::CHARSET_DEFAULT);
};

} // ns gsm

#endif // __MWIN_FONTPROV_HPP
