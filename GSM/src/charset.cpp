#include "../charset.hpp"

namespace gsm {

void
CharacterSet::add(unicode_t first, unsigned num_chars)
{
    for (ranges_t::iterator it = ranges.begin(); it != ranges.end(); it ++)
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
            ranges.insert(it, Range(first, num_chars));
            break; // done
        }
    }
}

const CharacterSet::Iterator
CharacterSet::find(unicode_t ch) const
{
    for (unsigned ir = 0; ir < ranges.size(); ir ++) {
        int ich = ranges[ir].index(ch);
        if (ich >= 0) {
            return Iterator(*this, ir, ich);
        }
    }
    return Iterator(*this);
}

void
CharacterSet::collapse_from(ranges_t::iterator & from)
{
    ranges_t::iterator it1 = from, it2 = from + 1;
    while (it2 != ranges.end() && (it1->first + it1->num_chars) >= it2->first)
    {
        // Merge successor range with current one
        it2->num_chars += it2->first - it1->first;
        it2->first = it1->first;
        // Discard current range
        ranges.erase(it1);
        // On to next
        it1 = it2 ++;
    }
}

} // ns gsm
