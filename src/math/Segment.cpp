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

double Segment::pointDistance(Math::Point p) const
{
	//http://www.codeguru.com/forum/printthread.php?t=194400

	if (a == b) return distance(a, p);

	//r is between 0 and 1 if the closest point on the line is within the segment
	double rNumerator = (p.x-a.x)*(b.x-a.x) + (p.y-a.y)*(b.y-a.y);
	double rDenominator = (b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y);
	double r = rNumerator / rDenominator;

	if (r >= 0 && r <= 1) {
		//the closest point is within the segment (as opposed to on the same line but outside the segment's endpoints).
		//complete the point-line distance formula
		return fabs((a.y-p.y)*(b.x-a.x) - (a.x-p.x)*(b.y-a.y) / rDenominator) * sqrt(rDenominator);
	} else {
		//the closest point on the line is outside the segment, so the closest point on the segment is one of the endpoints.
		double sqDistA = distanceSq(a, p);
		double sqDistB = distanceSq(b, p);

		return sqrt(std::min(sqDistA, sqDistB)); //return the closer endpoint
	}
}

} //namespace Math
