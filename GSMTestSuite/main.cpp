#include <exception>
#include <iostream>
#include <GSM/gsm.hpp>

int
main(int argc, char *argv[])
{
    try {
        gsm::Session sess;

        sess.openWindow();

        sess.run();

        return 0;
    }
    catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 1; // error
}
