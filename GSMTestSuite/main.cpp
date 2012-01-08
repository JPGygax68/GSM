#include <exception>
#include <iostream>
#include <GSM/gsm.hpp>
#include <GSM/isurface.hpp>

int
main(int argc, char *argv[])
{
    try {
        gsm::Session sess;

        gsm::ISurface *win = sess.openWindow();

        sess.run();

        return 0;
    }
    catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 1; // error
}
