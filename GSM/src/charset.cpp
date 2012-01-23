#include "../charset.hpp"

namespace gsm {

const CharacterSet &
CharacterSet::LATIN1()
{
    static CharacterSet charset;

    charset.addRange(0x21, 0x7E);
    charset.addRange(0xA1, 0xFF);

    return charset;
}

void
CharacterSet::add(unicode_t first, unsigned num_chars)
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
            else {
                range.num_chars += (range.first - first);
                range.first = first;
            }
            break; // done
        }
        // .. new range intersects at or appends to it ?
        else if (first >= range.first && first < (range.first + range.num_chars)) {
            // New range ends after existing one?
            if ((first + num_chars) > (range.first + range.num_chars)) {
                range.num_chars = (first - range.first) + num_chars;
                // Merge with collating next ranges
                collapse_from(it);
            }
            else ; // nothing to do, existing range swallows new one
            break; // done
        }
        // .. new range must be inserted before this one?
        else if (first < range.first) {
            _ranges.insert(it, Range(first, num_chars));
            break; // done
        }
    }
}

void
CharacterSet::add(const Range &range)
{
    _ranges.push_back(range);
}

void
CharacterSet::addRange(unicode_t first, unicode_t last)
{
    add(first, last - first + 1);
}

const CharacterSet::iterator
CharacterSet::find(unicode_t ch) const
{
    for (unsigned ir = 0; ir < _ranges.size(); ir ++) {
        int ich = _ranges[ir].index(ch);
        if (ich >= 0) {
            return iterator(*this, ir, ich);
        }
    }
    return iterator(*this);
}

void
CharacterSet::collapse_from(ranges_t::iterator & from)
{
    ranges_t::iterator it1 = from, it2 = from + 1;
    while (it2 != _ranges.end() && (it1->first + it1->num_chars) >= it2->first)
    {
        // Merge successor range with current one
        it2->num_chars += it2->first - it1->first;
        it2->first = it1->first;
        // Discard current range
        _ranges.erase(it1);
        // On to next
        it1 = it2 ++;
    }
}

} // ns gsm
