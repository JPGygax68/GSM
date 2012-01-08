#include <exception>
#include <iostream>
#include <GSM/gsm.hpp>
#include <GSM/isurface.hpp>
#include <GSM/ievent.hpp>

int
main(int argc, char *argv[])
{
    using namespace gsm;

    try {
        ISessionManager *sm = static_cast<ISessionManager*>( findComponent("SessionManager") );

        ISurface *win = sm->openWindow(10, 10, 800, 600);
        ISurface *win2 = sm->openWindow(20, 200, 800, 600);

        win->show();
        win2->show();

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
