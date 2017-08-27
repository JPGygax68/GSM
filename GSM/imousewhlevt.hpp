/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2017 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#pragma once

#include "gfxtypes.hpp"
#include "dll.h"
#include "gfxtypes.hpp"


namespace gsm {

    class GSM_API IMouseWheelEvent {
    public:
        virtual int delta() = 0;
    };

} // ns gsm