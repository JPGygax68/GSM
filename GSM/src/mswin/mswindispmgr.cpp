#include "../../idispmgr.hpp"
#include "../../compmgr.hpp"

namespace gsm {

class Registrator;

class MSWinDisplayManager: public IDisplayManager {
private:
    MSWinDisplayManager();
    friend class Registrator;
};

REGISTER_COMPONENT("DisplayManager", MSWinDisplayManager);

MSWinDisplayManager::MSWinDisplayManager()
{
}

} // ns gsm
