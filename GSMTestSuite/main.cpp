#include <exception>
#include <iostream>
#include <GSM/gsm.hpp>
#include <GSM/isurface.hpp>
#include <GSM/iwindow.hpp>

class MyWindow: public gsm::IWindow {
public:
    MyWindow(gsm::ISessionManager *sm, int x, int y, int w, int h, const char *caption = NULL) {
        surf = sm->openWindow(x, y, w, h, caption, this);
        surf->show();
    }

    virtual void
    onResize(int w, int h) {
    }

    virtual void
    onPaint(gsm::ICanvas *cnv) {
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
    onManagementEvent(gsm::IEvent *evt) {
        return false; }

private:
    gsm::ISurface *surf;
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
