/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_IPTRBTNEVT_HPP
#define __GSM_IPTRBTNEVT_HPP

#include "dll.h"
#include "gfxtypes.hpp"

namespace gsm {

    class GSM_API IPointerButtonEvent {
    public:
        enum Button { NONE = 0, LEFT, RIGHT, MIDDLE, WHEEL, EXTENDED_1, EXTENDED_2 };

        virtual const Position position() = 0;

        virtual Button button() = 0;
        virtual bool pressed() = 0;
        virtual bool released() = 0;
        virtual bool doubleClick() = 0;
    };

} // ns gsm

#endif // __GSM_IPTRBTNEVT_HPP
