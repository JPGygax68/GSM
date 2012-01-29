#ifndef __GSM_UTIL_FORMAT_HPP
#define __GSM_UTIL_FORMAT_HPP

#include <string>
#include <stdarg.h>

const std::string
format(const char *fmt, ...);

const std::wstring
format(const wchar_t *fmt, ...);

#endif // __GSM_UTIL_FORMAT_HPP
