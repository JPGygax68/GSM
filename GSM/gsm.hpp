#ifndef __GSM_HPP
#define __GSM_HPP

#include "compmgr.hpp"          // the Component Manager is the starting point and is always needed
#include "isessionmgr.hpp"      // cannot do much at all without a Session Manager

namespace gsm {

class ISessionManager;
class IFontProvider;

void init();

void shutdown();

ISessionManager *sessionManager();

IFontProvider *fontProvider();

} // ns gsm

#endif // __GSM_HPP
