#include "../idispmgr.hpp"
#include "../compmgr.hpp"
#include "../session.hpp"

namespace gsm {

Session::Session()
{
    dispman = static_cast<IDisplayManager*>( findComponent("DisplayManager") );
}

ISurface *
Session::openWindow()
{
    return dispman->openWindow();
}

void
Session::run()
{
}

} // ns gsm
