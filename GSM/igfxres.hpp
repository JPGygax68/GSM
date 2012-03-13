/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

// TODO: FUTURE EXTENSION. This file is not currently being used.

#ifndef __GSM_GFXRES_HPP
#define __GSM_GFXRES_HPP

#include "dll.h"

namespace gsm {

    class IBoundGraphicsResource;

    class GSM_API IGraphicsResource {
    public:
        virtual void * bind(int vidMemCtxID) = 0;
        virtual bool isBound(int vidMemCtxID) = 0;
        virtual void releaseBinding(void *bgres) = 0;
    };

} // ns gsm

#endif // __GSM_GFXRES_HPP
