/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

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
    virtual unsigned
    getScreenResolution(int num);

    virtual ISurface *
    openWindow(int x, int y, int w, int h, const char *caption, IDisplay *window, ISurface::Attributes caps = 0);

    virtual ISurface *
    openScreen(int num, IDisplay *screen, ISurface::Attributes caps = 0);

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
