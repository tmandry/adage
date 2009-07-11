#ifndef SEGMENT_H_
#define SEGMENT_H_

#include <QLineF>
#include "math/Point.h"

namespace Math {

// A line segment struct. Keeps a and b consistent; never swaps them.
struct Segment
{
	Point a, b;

	Segment(Point A, Point B)
		:	a(A), b(B)
		{}
	Segment()
		{}
	~Segment() {}

	Point midpoint() const { return Point((a.x+b.x)/2.0, (a.y+b.y)/2.0); }

	enum PointRelation {
		left,
		right,
		on,
		inLine
	};
	PointRelation classifyPoint(Point p) const;

	operator QLineF() const
	{
		return QLineF(a.x,a.y, b.x,b.y);
	}

	bool operator==(const Segment& rhs) const {return (a==rhs.a && b==rhs.b) || (a==rhs.b && b==rhs.a);}
	bool operator!=(const Segment& rhs) const {return (a!=rhs.a || b!=rhs.b) && (a!=rhs.b || b!=rhs.a);}
};

inline double length(const Segment& s) {return distance(s.a, s.b);}
inline double lengthSq(const Segment& s) {return distanceSq(s.a, s.b);}

bool segmentIntersection(Segment s1, Segment s2, Point& intersection, double& aLocal);

} // namespace Math

#endif /*SEGMENT_H_*/
