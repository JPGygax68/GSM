/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_HPP
#define __GSM_HPP

#include "dll.h"
#include "compmgr.hpp"          // the Component Manager is the starting point and is always needed
#include "isessionmgr.hpp"      // cannot do much at all without a Session Manager

namespace gsm {

    class ISessionManager;
    class IFontProvider;

    void GSM_API init();

    void GSM_API shutdown();

    ISessionManager GSM_API * sessionManager();

    IFontProvider GSM_API * fontProvider();

} // ns gsm

#endif // __GSM_HPP
