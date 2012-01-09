#include <exception>
#include <iostream>
#include <GSM/gsm.hpp>
#include <GSM/isurface.hpp>
//#include <GSM/ievent.hpp>
#include <GSM/iwindow.hpp>

class MyWindow: public gsm::IWindow {
public:
    MyWindow(gsm::ISessionManager *sm, int x, int y, int w, int h) {
        surf = sm->openWindow(x, y, w, h, this);
        surf->show();
    }

    virtual void
    onResize(int w, int h) {
    }

private:
    gsm::ISurface *surf;
};

int
main(int argc, char *argv[])
{
    using namespace gsm;

    try {
        ISessionManager *sm = static_cast<ISessionManager*>( findComponent("SessionManager") );

        MyWindow win1(sm, 10, 10, 800, 600);
        MyWindow win2(sm, 30, 200, 800, 600);

        while (true) {
            sm->processNextEvent();
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
