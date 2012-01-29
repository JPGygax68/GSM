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

    va_end(list);

    return std::string(buffer);
}

const std::wstring
format(const wchar_t *fmt, ...)
{
    va_list list;

    va_start(list, fmt);

    wchar_t buffer[2048+1];
    vswprintf(buffer, 2048, fmt, list);

    va_end(list);

    return std::wstring(buffer);
}
