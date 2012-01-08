#include <exception>
#include <iostream>
#include <GSM/gsm.hpp>
#include <GSM/isurface.hpp>

int
main(int argc, char *argv[])
{
    try {
        gsm::ISessionManager *sm = static_cast<gsm::ISessionManager*>( gsm::findComponent("SessionManager") );

        gsm::ISurface *win = sm->openWindow();

        sm->run();

        return 0;
    }
    catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 1; // error
}
