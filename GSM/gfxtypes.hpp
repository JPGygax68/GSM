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

} // ns gsm

#endif // __GSM_GFXTYPES_HPP
