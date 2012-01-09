#ifndef __GSM_MSWINERR_HPP
#define __GSM_MSWINERR_HPP

#include <exception>
#include "../../util/format.hpp"

namespace gsm {

class EMSWinError: public std::exception {
public:
    EMSWinError(int err, const char *context = NULL);
};

} // ns gsm

// TODO: make variations that only check in debug builds
#define CHECK(func, args) if (! func args ) throw EMSWinError(GetLastError(), #func);

#endif // __GSM_MSWINERR_HPP
