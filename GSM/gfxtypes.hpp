#ifndef __GSM_GFXTYPES_HPP
#define __GSM_GFXTYPES_HPP

#include <limits>

namespace gsm {

struct BoundingBox {
	int		xMin, yMin;
	int		xMax, yMax;

    unsigned width()  const { return (unsigned) (-xMin + xMax); }
	unsigned height() const { return (unsigned) (-yMin + yMax); }

    BoundingBox() { reset(); }
	
    void reset() { xMin = yMin = xMax = yMax = 0; }
	
    static int mymin(int x, int y) { return x < y ? x : y; }
    static int mymax(int x, int y) { return x > y ? x : y; }

    BoundingBox & merge(const BoundingBox &other, int x = 0, int y = 0) {
        xMin = mymin(xMin, x + other.xMin);
        yMin = mymin(yMin, y + other.yMin);
        xMax = mymax(xMax, x + other.xMax);
        yMax = mymax(yMax, y + other.yMax);
        return *this;
    }

    BoundingBox & inflate(int x, int y) {
        xMin -= x; xMax += x;
        yMin -= y; yMax += y; 
        return *this; }

	BoundingBox & operator | (const BoundingBox &other) {
		return merge(other, 0, 0);
	}
};

struct Extents {
    unsigned w, h;
    Extents(unsigned w_, unsigned h_): w(w_), h(h_) {}
    Extents(): w(0), h(0) {}
};

struct Position { 
    int x, y; 
};

struct Rect {
    int x, y;
    unsigned w, h;
};

} // ns gsm

#endif // __GSM_GFXTYPES_HPP
