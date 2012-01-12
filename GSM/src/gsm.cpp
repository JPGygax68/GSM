#include "../compmgr.hpp"
#ifdef WIN32
#include "mswin/mswinsessmgr.hpp"
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

} // ns gsm
