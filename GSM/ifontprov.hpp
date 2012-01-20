#ifndef __GSM_IFONTPROV_HPP
#define __GSM_IFONTPROV_HPP

#include <string>
#include "util/bitset.hpp"

#include "ifont.hpp"

namespace gsm {

    class IFontProvider {
    public:

        /** This call selects a font by its general type. It returns an
            IFont interface that identifies the selected font.
	        IMPORTANT: the implementation is free to ignore both the font 
	        type *and* the height specification! This makes sense when the
	        display driver is simplified to the extreme, either because
	        of platform limitations (cell phones ?) or to minimize the 
	        footprint.
            The IFont pointer is owned by the caller and should be deleted
            when no longer needed.
	        */
        virtual IFont * getFont(IFont::Type type = IFont::ANY,
	        const std::string &name = "", 
            size_t height = 0, 
            IFont::Attributes attribs = 0) = 0;

    };

} // ns gsm

#endif // __GSM_IFONTPROV_HPP
