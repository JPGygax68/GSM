/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_IFONTPROV_HPP
#define __GSM_IFONTPROV_HPP

#include <string>

#include "dll.h"
#include "icomp.hpp"
#include "util/bitset.hpp"

#include "ifont.hpp"

namespace gsm {

    class GSM_API IFontProvider: public IComponent {
    public:

        enum Unit { PIXELS = 0, POINTS };

        /** This call selects a font by its general type. It returns an
            IFont interface that identifies the selected font.
	        IMPORTANT: the implementation is free to ignore both the font 
	        type *and* the height specification! This makes sense when the
	        font provider is simplified to the extreme, either because
	        of platform limitations (cell phones ?) or to minimize the 
	        footprint.
            The IFont pointer is owned by the caller and should be deleted
            when no longer needed.
	        */
        virtual IFont * getFont(IFont::Type type = IFont::ANY,
	        const std::string &name = "", 
            size_t height = 0, 
            IFont::Attributes attribs = 0,
			IFont::CharSet charset = IFont::CHARSET_DEFAULT
            ) = 0;

    };

} // ns gsm

#endif // __GSM_IFONTPROV_HPP
