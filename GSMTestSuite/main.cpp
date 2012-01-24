#include <exception>
#include <iostream>
#include <cassert>
#include <GSM/gsm.hpp>
#include <GSM/isurface.hpp>
#include <GSM/idisplay.hpp>

class MyWindow: public gsm::IDisplay {
public:

    MyWindow(gsm::ISessionManager *sm, int x, int y, int w, int h, const char *caption = NULL) {
        closed = false;
        surf = sm->openWindow(x, y, w, h, caption, this, gsm::ISurface::SUPPORTS_OPENGL);
        surf->show();
    }

    virtual void
    onResize(int w, int h) {
    }

    virtual void
    onInit()
    {
    }

    virtual void
    onPaint(gsm::ICanvas *cnv) {
        assert(!closed);
        doPaint();
    }

    virtual bool
    onKeyboardEvent(gsm::IKeyboardEvent *evt) {
        return false; }

    virtual bool
    onPointerMotionEvent(gsm::IPointerMotionEvent *evt) {
        return false; }

    virtual bool
    onPointerButtonEvent(gsm::IPointerButtonEvent *evt) {
        return false; }

    virtual bool
    onClose() {
        closed = true;
        return true; }

    void
    paint() {
        if (!closed) {
            surf->select();
            doPaint();
        }
    }

private:

    void
    doPaint() {
    }

    gsm::ISurface *surf;
    bool closed;
};

int
main(int argc, char *argv[])
{
    using namespace gsm;

    try {
        gsm::init();
        ISessionManager *sm = static_cast<ISessionManager*>( findComponent("SessionManager") );

        MyWindow win1(sm, 10, 10, 800, 600);
        MyWindow win2(sm, 30, 200, 800, 600, "Window 2");

        while (true) {
            win1.paint();
            win2.paint();
            sm->fetchNextEvent();
            (void) sm->getEvent();
            if (sm->mustQuit() )
                break;
        }

        return 0;
    }
    catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 1; // error
}
