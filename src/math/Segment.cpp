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

	if ((ua >= 0 && ua <= 1.0) && (ub >= 0 && ub <= 1.0)) {
		intersection.x = s1.a.x + ua*(s1.b.x-s1.a.x);
		intersection.y = s1.a.y * ua*(s1.b.y-s1.a.y);
		aLocal = ua;
		return true;
	} else
		return false;
}

//returns whether a point is left, right, inLine with, or on the Segment (as if you were facing from a to b)
Segment::PointRelation Segment::classifyPoint(Math::Point p) const
{
	//note these reflect the INVERTED y-axis
	//first deal with horizontal & vertical special cases
	//vertical
	if (a.x == b.x) {
		if (b.y < a.y) { //going up
			if (p.x > a.x) return right;
			else if (p.x < a.x) return left;
			else if (b.y <= p.y && p.y <= a.y) return on;
			else return inLine;
		}

		assert(a.y != b.y);
		//going down
		if (p.x < a.x) return right;
		else if (p.x > a.x) return left;
		else if (a.y <= p.y && p.y <= b.y) return on;
		else return inLine;
	}

	//horizontal
	if (a.y == b.y) {
		if (b.x > a.x) { //going right
			if (p.y > a.y) return right;
			else if (p.y < a.y) return left;
			else if (a.x <= p.x && p.x <= b.x) return on;
			else return inLine;
		}

		//going left
		if (p.y < a.y) return right;
		else if (p.y > a.y) return left;
		else if (b.x <= p.x && p.x <= a.x) return on;
		else return inLine;
	}

	//not horizontal or vertical
	double m = (b.y - a.y) / (b.x - a.x);
	double c = a.y - m*a.x;

	//mx - y + c: Result is - if above, + if below: if positive slope, above is left and below is right
	//0 if on line.
	//return opposite if segment goes down (b.y > a.y)
	double result = m*p.x - p.y + c;
	if ((m > 0 && result < 0) || (m < 0 && result > 0)) { if (a.y > b.y) return left; else return right; }
	else if ((m > 0 && result > 0) || (m < 0 && result < 0)) { if (a.y > b.y) return right; else return left; }
	else if ((a.x <= p.x && p.x <= b.x) || (b.x <= p.x && p.x <= a.x)) return on;
	else return inLine;
}

} //namespace Math
