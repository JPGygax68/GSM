/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_IDISPLAY_HPP
#define __GSM_IDISPLAY_HPP

#include "dll.h"
#include "icanvas.hpp"

namespace gsm {

class IKeyboardEvent;
class IPointerMotionEvent;
class IPointerButtonEvent;

class GSM_API IDisplay {
public:
    virtual void onInit() = 0;
    virtual void onResize(int w, int h) = 0;
    virtual bool onPaint(ICanvas *canvas, int vidCtxID) = 0;
    virtual bool onClose() = 0;
    virtual bool onKeyboardEvent(IKeyboardEvent *evt) = 0;
    virtual bool onPointerMotionEvent(IPointerMotionEvent *evt) = 0;
    virtual bool onPointerButtonEvent(IPointerButtonEvent *evt) = 0;
};

} // ns gsm

#endif // __GSM_IDISPLAY_HPP
