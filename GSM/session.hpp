#ifndef __GSM_SESSION_HPP
#define __GSM_SESSION_HPP

namespace gsm {

class IDisplayManager;
class ISurface;

class Session {
public:
    Session();

    ISurface * 
    openWindow();

    void
    run();

private:
    IDisplayManager *dispman;
};

} // ns gsm

#endif // __GSM_SESSION_HPP
