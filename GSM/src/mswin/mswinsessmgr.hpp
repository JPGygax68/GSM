#ifndef MSWIN_SESSMGR_HPP
#define MSWIN_SESSMGR_HPP

#include <windows.h>
#include <set>

#include "mswinevt.hpp"
#include "../../isessionmgr.hpp"

namespace gsm {

class MSWinSurface;

class MSWinSessionManager: public ISessionManager {
private:
    MSWinSessionManager();
    ~MSWinSessionManager();
    friend class Registrator;

public:
    virtual ISurface *
    openWindow(int x, int y, int w, int h, const char *caption, IWindow *window);

    virtual ISurface *
    openScreen(int num, ISurface::Attributes attr, IScreen *scr);

    virtual bool
    fetchNextEvent();

    virtual IEvent *
    getEvent();

    virtual bool
    mustQuit();    // someone has asked to close session

    MetaKeys
    getMetaKeyStates();

public: // internal
    void
    closeMsgReceived(HWND hWnd);

public:
    static void
    registerIt() { registerComponent("SessionManager", new MSWinSessionManager() ); }

private:
    typedef std::set<MSWinSurface*> surfacelist_t;
    typedef surfacelist_t::iterator surface_iterator_t;

    surfacelist_t surfaces;
    bool have_evt;
    MSWinEvent evt;
    bool quit;
};

} // ns gsm

#endif // MSWIN_SESSMGR_HPP
