#ifndef __GSM_IWINDOW_HPP
#define __GSM_IWINDOW_HPP

#include "icanvas.hpp"
#include "idisplay.hpp"

namespace gsm {

class IWindow {
public:
    virtual void onResize(int w, int h) = 0;
    virtual void onPaint(ICanvas *canvas) = 0;
    virtual bool onManagementEvent(IEvent *evt) = 0;
};

} // ns gsm

#endif // __GSM_IWINDOW_HPP
