#include <Windows.h>
#include "mswinerr.hpp"

namespace gsm {

const std::string makeErrorString(int err);

EMSWinError::EMSWinError(int err, const char *context)
    : std::exception( (makeErrorString(err) + "in function " + context).c_str() )
{}

static const std::string
makeErrorString(int err)
{
    LPSTR s;

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        err,
        0,
        (LPSTR)&s,
        0,
        NULL) != 0)
    {
        LPSTR p = strchr(s, '\r');
        if (p) *p = '\0';
        std::string res = s;
        LocalFree(s);
        return res;
    }
    else {
        return std::string("(Failed to retrieve error string");
    }
}

} // ns gsm
