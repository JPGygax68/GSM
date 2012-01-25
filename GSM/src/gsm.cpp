#include "../compmgr.hpp"
#ifdef WIN32
#include "mswin/mswinsessmgr.hpp"
#include "mswin/mswinfontprov.hpp"
#endif
#include "../gsm.hpp"

namespace gsm {

void shutdown();

void
init()
{
    atexit( shutdown );
#ifdef WIN32
    MSWinSessionManager::registerIt();
    MSWinFontProvider::registerIt();
#endif
}

void
shutdown()
{
    unregisterAll();
}

ISessionManager *
sessionManager()
{
    static ISessionManager *sm = NULL;
    if (sm == NULL) {
        sm = static_cast<ISessionManager*>(findComponent("SessionManager"));
    }
    return sm;
}

IFontProvider *
fontProvider()
{
    static IFontProvider *fp = NULL;
    if (fp == NULL) {
        fp = static_cast<IFontProvider*>(findComponent("FontProvider"));
    }
    return fp;
}

} // ns gsm
