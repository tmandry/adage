#ifndef POLYGON_H_
#define POLYGON_H_

#include <vector>
#include <set>
#include <cassert>
#include "math/Point.h"
#include "math/Segment.h"

namespace Math
{

class Polygon
{
public:
	Polygon() {}
	Polygon(const Point* start, const Point* end);
	virtual ~Polygon() {}

	const std::vector<Point>& points() const { return mPoints; }
	Point centerPoint() const { assert(!mPoints.empty()); return mCenter; }
	Segment edge(unsigned int i) const
	{
		assert(i < mPoints.size());
		if (i < mPoints.size()-1) return Segment(mPoints[i], mPoints[i+1]);
		else return Segment(mPoints[mPoints.size()-1], mPoints[0]);
	}

private:
	std::vector<Point> mPoints;
	Point mCenter;
};


class ConvexPolygon : public Polygon
{
public:
	ConvexPolygon() {}
	ConvexPolygon(const Point* start, const Point* end);
	virtual ~ConvexPolygon() {}

	bool contains(Point p) const;

private:
	class EdgeSort
	{
	public:
	    bool operator()(const Segment& lhs, const Segment& rhs) const
	    {
	    	if (lhs.b.x == rhs.b.x) return lhs.b.y < rhs.b.y;
	    	else return lhs.b.x < rhs.b.x;
	    }
	};

	typedef std::multiset<Segment, EdgeSort> EdgeSet;
	EdgeSet mPointsUpper;
	EdgeSet mPointsLower;

	enum Result {
		cross,
		noCross,
		in,
		out
	};
	static inline Result rayTest(Point p, const EdgeSet& points);
};

}

#endif /* POLYGON_H_ */
