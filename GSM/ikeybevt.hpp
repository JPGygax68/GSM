/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_IKEYBEVT_HPP
#define __GSM_IKEYBEVT_HPP

#include "types.hpp"
#include "ievent.hpp"

namespace gsm {

class IKeyboardEvent {
public:
    enum Keycode { SHIFT = 16, CTRL = 17, ALT = 18 };

    typedef int keycode_t;

    virtual bool down() = 0;
    virtual bool up() = 0;
    virtual bool isCharacter() = 0;
    virtual unicode_t unicode() = 0;
    virtual keycode_t keyCode() = 0;
};

} // ns gsm

#endif // __GSM_IKEYBEVT_HPP
