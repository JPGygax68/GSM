#ifndef __GSM_IWINDOW_HPP
#define __GSM_IWINDOW_HPP

namespace gsm {

class IWindow {
public:
    virtual void onResize(int w, int h) = 0;
};

} // ns gsm

#endif // __GSM_IWINDOW_HPP
