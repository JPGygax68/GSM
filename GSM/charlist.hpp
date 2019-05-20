/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_CHARLIST_HPP
#define __GSM_CHARLIST_HPP

#include <deque>
#include <cassert>
#include "types.hpp"

namespace gsm {

class CharacterList {
public:
    struct Range { 
        unicode_t first;
        unsigned num_chars;
        Range(unicode_t first_, unsigned count_): first(first_), num_chars(count_) {}
        int index(unicode_t ch) const { 
            if (ch < first) return -1;
            else if (ch > (first + num_chars)) return -1;
            else return ch - first; }
    };
    typedef std::deque<Range> ranges_t;

    class iterator {
    public:

        iterator & operator = (const iterator & src) { 
            set = src.set; 
            irange = src.irange;
            ichar = src.ichar;
            return *this;
        }
        iterator & operator ++ (int) {
            assert(irange < set->_ranges.size());
            if (++ichar >= set->_ranges[irange].num_chars) {
                irange ++;
                ichar = 0; 
            }
            return *this;
        }
        unicode_t operator * () {
            assert(irange < set->_ranges.size());
            assert(ichar < set->_ranges[irange].num_chars);
            return set->_ranges[irange].first + ichar;
        }

        bool operator == (const iterator &other) const {
            return irange == other.irange && ichar == other.ichar; }

        bool operator != (const iterator &other) const {
            return ! (other == *this); }

    private:
        iterator(const CharacterList &set_, unsigned irange_, unsigned ichar_)
            : set(&set_), irange(irange_), ichar(ichar_) {}
        iterator(const CharacterList &set_)
            : set(&set_), irange(static_cast<unsigned>(set_._ranges.size())), ichar(0) {}

        const CharacterList * set;
        unsigned irange, ichar;

        friend class CharacterList;
    };

    static const CharacterList & LATIN1();
    static const CharacterList & CHINESE();

    static const CharacterList * forCharSet(int charset);

    ranges_t & ranges() { return _ranges; }

    bool empty() const { return _ranges.empty(); }

    iterator begin() { return iterator(*this, 0, 0); }

    const iterator begin() const { return iterator(*this, 0, 0); }

    const iterator end() const { return iterator(*this); }

    const iterator find(unicode_t ch) const;

    void add(unicode_t first, unsigned num_chars = 1);

    void add(const Range &range);

    void addRange(unicode_t first, unicode_t last);

private:

    void collapse_from(ranges_t::iterator & from);

    ranges_t _ranges;
};

} // ns gsm

#endif // __GSM_CHARLIST_HPP
