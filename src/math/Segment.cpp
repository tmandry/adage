#include <cassert>
#include "Segment.h"

namespace Math {

bool segmentIntersection(Segment s1, Segment s2, Point& intersection, double& aLocal)
{
	//x1 + ua (x2 - x1) = x3 + ub (x4 - x3)
	//y1 + ua (y2 - y1) = y3 + ub (y4 - y3)

	double denom = ((s2.b.y - s2.a.y)*(s1.b.x - s1.a.x) - (s2.b.x - s2.a.x)*(s1.b.y - s1.a.y));
	if (denom == 0) return false; //parallel

	double ua =
		((s2.b.x - s2.a.x)*(s1.a.y - s2.a.y) - (s2.b.y - s2.a.y)*(s1.a.x - s2.a.x)) /
		denom;
	double ub =
		((s1.b.x - s1.a.x)*(s1.a.y - s2.a.y) - (s1.b.y - s1.a.y)*(s1.a.x - s2.a.x)) /
		denom;

	if ((ua > 0 && ua <= 1.0) && (ub > 0 && ub <= 1.0)) {
		intersection.x = s1.a.x + ua*(s1.b.x-s1.a.x);
		intersection.y = s1.a.y * ua*(s1.b.y-s1.a.y);
		aLocal = ua;
		return true;
	} else
		return false;
}

Segment::PointRelation Segment::classifyPoint(Math::Point p) const
{
	//first deal with horizontal & vertical special cases
	//vertical
	if (a.x == b.x) {
		if (b.y > a.y) { //going up
			if (p.x > a.x) return right;
			else if (p.x < a.x) return left;
			else return on;
		}

		assert(a.x != b.y);
		//going down
		if (p.x < a.x) return right;
		else if (p.x > a.x) return left;
		else return on;
	}

	//horizontal -- note these reflect the INVERTED y-axis
	if (a.y == b.y) {
		if (b.x > a.x) { //going right
			if (p.y > a.y) return right;
			else if (p.y < a.y) return left;
			else return on;
		}

		//going left
		if (p.y < a.y) return right;
		else if (p.y > a.y) return left;
		else return on;
	}

	//not horizontal or vertical
	double m = (b.y - a.y) / (b.x - a.x);
	double c = a.y - m*a.x;

	//mx - y + c: Result is + if right side, - if left side, 0 if on line
	double result = m*p.x - p.y + c;
	if (result < 0) return left;
	else if (result > 0) return right;
	else return on;
}

} //namespace Math
