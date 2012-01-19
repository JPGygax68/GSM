#ifndef __GSM_CHARSET_HPP
#define __GSM_CHARSET_HPP

#include <deque>
#include <cassert>
#include "types.hpp"

namespace gsm {

class CharacterSet {
public:
    class Iterator {
    public:
        Iterator & operator ++ (int) {
            assert(irange < set.ranges.size());
            if (++ichar >= set.ranges[irange].num_chars) {
                irange ++;
                ichar = 0; 
            }
            return *this;
        }
        unicode_t operator * () {
            assert(irange < set.ranges.size());
            assert(ichar < set.ranges[irange].num_chars);
            return set.ranges[irange].first + ichar;
        }

        bool operator == (const Iterator &other) const {
            return irange == other.irange && ichar == other.ichar; }

        bool operator != (const Iterator &other) const {
            return ! (other == *this); }

    private:
        Iterator(const CharacterSet &set_, unsigned irange_, unsigned ichar_)
            : set(set_), irange(irange_), ichar(ichar_) {}
        Iterator(const CharacterSet &set_)
            : set(set_), irange(set_.ranges.size()), ichar(0) {}

        const CharacterSet & set;
        unsigned irange, ichar;

        friend class CharacterSet;
    };

    const Iterator end() const { return Iterator(*this); }

    const Iterator find(unicode_t ch) const;

    void add(unicode_t first, unsigned num_chars);

private:
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

    void collapse_from(ranges_t::iterator & from);

    ranges_t ranges;
};

} // ns gsm

#endif // __GSM_CHARSET_HPP
