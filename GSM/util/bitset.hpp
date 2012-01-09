#ifndef __GSM_UTIL_BITSET_HPP
#define __GSM_UTIL_BITSET_HPP

// TODO: use an include to handle this ?
#ifndef GPCUINS
#define GPCUINS gui
#endif

namespace gsm {

/** TODO: iterators ?
	*/
template <typename Enum>
class bitset {
public:
	bitset() { bits = 0; }
	bitset(Enum e) { bits = 0; set(e); }
	bitset(unsigned _bits) { bits = _bits; }
	bitset(const bitset & from) { bits = from.bits; }
	bitset & operator = (const bitset & from) { bits = from.bits; return *this; }
	void set(Enum e) { 
		bits |= (1 << (unsigned)e); }
	void clear(Enum e) { bits &= ~(1 << (unsigned)e); }
	bool toggle(Enum e) { bits ^= (1 << (unsigned)e); return test(e); }
	bool test(Enum e) const { return (bits & (1 << (unsigned)e)) != 0; }
	bool contains_all(const bitset & other) const { return (bits & other.bits) == other.bits; }
	bool contains_any(const bitset & other) const { return (bits & other.bits) != 0; }
	bool operator == (const bitset & other) const { return bits == other.bits; }
	bitset & operator + (Enum e) { set(e); return *this; }
	bitset & operator - (Enum e) { clear(e); return *this; }
	operator unsigned () const { return bits; }
	operator unsigned & () { return bits; }
	//bitset & operator = (unsigned _bits) { bits = _bits; return *this; }
private:
	unsigned bits;
};

} // ns gsm

#endif // __GSM_UTIL_BITSET_HPP
