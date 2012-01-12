#ifndef __GSM_IDISPLAY_HPP
#define __GSM_IDISPLAY_HPP

#include "icanvas.hpp"
#include "idisplay.hpp"

namespace gsm {

class IKeyboardEvent;
class IPointerMotionEvent;
class IPointerButtonEvent;

class IDisplay {
public:
    virtual bool onKeyboardEvent(IKeyboardEvent *evt) = 0;
    virtual bool onPointerMotionEvent(IPointerMotionEvent *evt) = 0;
    virtual bool onPointerButtonEvent(IPointerButtonEvent *evt) = 0;
};

} // ns gsm

#endif // __GSM_IDISPLAY_HPP
