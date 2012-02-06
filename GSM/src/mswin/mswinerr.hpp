/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_MSWINERR_HPP
#define __GSM_MSWINERR_HPP

#include <exception>
#include "../../util/format.hpp"

namespace gsm {

class EMSWinError: public std::exception {
public:
    EMSWinError(int err, const char *context = NULL);
};

} // ns gsm

// TODO: make variations that only check in debug builds
#define CHECK(func, args) \
    if (! func args ) \
        throw EMSWinError(GetLastError(), #func);

#endif // __GSM_MSWINERR_HPP
