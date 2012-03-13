/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_IBITMAP_HPP
#define __GSM_IBITMAP_HPP

#include "dll.h"

namespace gsm {

    class GSM_API IBitmap {
    public:
        virtual ~IBitmap() {}
        virtual unsigned width() const = 0;
        virtual unsigned height() const = 0;
        virtual const void * data() const = 0;
    };

} // ns gsm

#endif // __GSM_IBITMAP_HPP
