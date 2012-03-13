/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_IPTRMOVEVT_HPP
#define __GSM_IPTRMOVEVT_HPP

#include "dll.h"

#include "gfxtypes.hpp"

namespace gsm {

    class GSM_API IPointerMotionEvent {
    public:

        virtual const Position position() = 0;
    };

} // ns gsm

#endif // __GSM_IPTRMOVEVT_HPP
