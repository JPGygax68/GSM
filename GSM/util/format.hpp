/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_UTIL_FORMAT_HPP
#define __GSM_UTIL_FORMAT_HPP

#include <string>
#include <stdarg.h>

const std::string
format(const char *fmt, ...);

const std::wstring
format(const wchar_t *fmt, ...);

#endif // __GSM_UTIL_FORMAT_HPP
