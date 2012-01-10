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

} // ns gsm
