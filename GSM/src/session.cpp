#include "../session.hpp"
#include "../idispmgr.hpp"
#include "../compmgr.hpp"

namespace gsm {

Session::Session()
{
    IDisplayManager * dm = static_cast<IDisplayManager*>( findComponent("DisplayManager") );
}

void
Session::openWindow()
{
}

void
Session::run()
{
}

} // ns gsm
