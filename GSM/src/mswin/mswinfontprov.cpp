/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "mswinfont.hpp"
#include "mswinfontprov.hpp"

// TODO: caching!

namespace gsm {

IFont *
MSWinFontProvider::getFont(IFont::Type type, const std::string &name, unsigned int height, IFont::Attributes attribs, IFont::CharSet charset)
{
    return new MSWinFont(this, type, name, height, attribs, charset);
}

} // ns gsm
