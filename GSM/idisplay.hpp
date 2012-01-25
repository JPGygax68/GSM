#ifndef __GSM_IDISPLAY_HPP
#define __GSM_IDISPLAY_HPP

#include "icanvas.hpp"

namespace gsm {

class IKeyboardEvent;
class IPointerMotionEvent;
class IPointerButtonEvent;

class IDisplay {
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
