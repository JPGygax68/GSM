#ifndef __GSM_IEVENT_HPP
#define __GSM_IEVENT_HPP

namespace gsm {

class IDisplay;
class IKeyboardEvent;
class IPointerMotionEvent;
class IPointerButtonEvent;

class IEvent {
public:
    virtual IDisplay * target() = 0;

    virtual bool isQuit() = 0;
    virtual bool isKeyboard() = 0;
    virtual bool isPointerMotion() = 0;
    virtual bool isPointerButton() = 0;
    virtual bool isCloseWindow() = 0;
    virtual bool isWindowSize() = 0;
    virtual bool isWindowManagement() = 0;

    virtual IKeyboardEvent *asKeyboard() = 0;
    virtual IPointerMotionEvent *asPointerMotion() = 0;
    virtual IPointerButtonEvent *asPointerButton() = 0;

};

} // ns gsm

#endif // __GSM_IEVENT_HPP
