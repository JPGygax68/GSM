#ifndef __GSM_GFXTYPES_HPP
#define __GSM_GFXTYPES_HPP

namespace gsm {

struct BoundingBox {
	int		xMin, yMin;
	int		xMax, yMax;
	unsigned width()  const { return (unsigned) (-xMin + xMax); }
	unsigned height() const { return (unsigned) (-yMin + yMax); }
	BoundingBox() { reset(); }
	void reset() { xMin = yMin = xMax = yMax = 0; }
	BoundingBox & operator | (const BoundingBox &other) {
		if (other.xMin < xMin) xMin = other.xMin;
		if (other.xMax > xMax) xMax = other.xMax;
		if (other.yMin < yMin) yMin = other.yMin;
		if (other.yMax > yMax) yMax = other.yMax;
		return *this;
	}
};

struct Extents {
    unsigned w, h;
    Extents(unsigned w_, unsigned h_): w(w_), h(h_) {}
    Extents(): w(0), h(0) {}
};

struct Rect {
    int x, y;
    unsigned w, h;
};

} // ns gsm

#endif // __GSM_GFXTYPES_HPP
