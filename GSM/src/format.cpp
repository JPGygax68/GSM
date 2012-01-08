#include <string>
#include <cstdarg>
#include "../util/format.hpp"

const std::string
format(const char *fmt, ...)
{
    va_list list;

    va_start(list, fmt);

    char buffer[2048+1];
    vsnprintf(buffer, 2048, fmt, list);

    return std::string(buffer);
}
