/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#include "../charlist.hpp"
#include "../ifont.hpp"
#include "../types.hpp"

namespace gsm {

const CharacterList *
CharacterList::forCharSet(int charset)
{
    switch(charset) {
    case IFont::CHINESEBIG5:
    case IFont::GB2312:
        return & CHINESE();
    default: 
        return & LATIN1();
    }
}

const CharacterList &
CharacterList::LATIN1()
{
    static CharacterList charset;
    static bool init_done = false;

    if (!init_done) {
        charset.add(' ');
        charset.addRange(0x21, 0x7E);
        charset.addRange(0xA1, 0xFF);
        init_done = true;
    }

    return charset;
}

const CharacterList &
CharacterList::CHINESE()
{
    static CharacterList charset;
    static bool init_done = false;

    if (!init_done) {
        charset.addRange(0x4E00, 0x9FFF);
        init_done = true;
    }

    return charset;
}

void
CharacterList::add(unicode_t first, unsigned num_chars)
{
    for (ranges_t::iterator it = _ranges.begin(); it != _ranges.end(); it ++)
    {
        Range & range(*it);
        // New range intersects this one at start, or prepends to it?
        if (first < (range.first + range.num_chars) && (first + num_chars) >= range.first) {
            // New range covers existing one?
            if ((first + num_chars) > (range.first + range.num_chars)) {
                range.first = first;
                range.num_chars = num_chars;
                // Merge with collating next ranges
                collapse_from(it);
            }
            else if (first < range.first) {
                range.num_chars += (range.first - first);
                range.first = first;
            }
            return; // done
        }
        // .. new range intersects at or appends to it ?
        else if (first >= range.first && first <= (range.first + range.num_chars)) {
            // New range ends after existing one?
            if ((first + num_chars) > (range.first + range.num_chars)) {
                range.num_chars = (first - range.first) + num_chars;
                // Merge with collating next ranges
                collapse_from(it);
            }
            else ; // nothing to do, existing range swallows new one
            return; // done
        }
        // .. new range must be inserted before this one?
        else if (first < range.first) {
            _ranges.insert(it, Range(first, num_chars));
            return; // done
        }
    }
    // New Range comes after any existing one
    _ranges.push_back(Range(first, num_chars));
}

void
CharacterList::add(const Range &range)
{
    _ranges.push_back(range);
}

void
CharacterList::addRange(unicode_t first, unicode_t last)
{
    add(first, last - first + 1);
}

const CharacterList::iterator
CharacterList::find(unicode_t ch) const
{
    for (unsigned ir = 0; ir < _ranges.size(); ir ++) {
        int ich = _ranges[ir].index(ch);
        if (ich >= 0) {
            return iterator(*this, ir, ich);
        }
    }
    return iterator(*this);
}

/*  This is a "cleanup" routine that must be called after a range has been extended.
    It will merge the Range pointed by "from" with all following Ranges that it
    touches.
 */
void
CharacterList::collapse_from(ranges_t::iterator & from)
{
    ranges_t::iterator it1 = from, it2 = from + 1;
    while (it2 != _ranges.end() && (it1->first + it1->num_chars) >= it2->first)
    {
        // Current Range encompasses whole or more of successor ?
        if ((it1->first + it1->num_chars) >= (it2->first + it2->num_chars)) {
            // Copy current to successor 
            it2->first = it1->first;
            it2->num_chars = it1->num_chars;
        }
        else  // No, current Range just touches or partially overlaps successor
        {
            it2->num_chars += it2->first - it1->first;
            it2->first = it1->first;
        }
        // On to next
        it1 = it2 ++;
    }
    // Discard deleted ranges
    _ranges.erase(from, it1);
}

} // ns gsm
