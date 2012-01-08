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

#endif // __GSM_MSWINERR_HPP
