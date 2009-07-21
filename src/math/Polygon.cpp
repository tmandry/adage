#include <cassert>
#include <iostream>
#include "Polygon.h"

namespace Math
{

Polygon::Polygon(QList<Point> points)
	:	mPoints(points.begin(), points.end())
{
	foreach (Math::Point point, points) {
		mCenter.x += point.x;
		mCenter.y += point.y;
	}

	mCenter.x /= mPoints.size();
	mCenter.y /= mPoints.size();
}

ConvexPolygon::ConvexPolygon(QList<Point> points)
	:	Polygon(points)
{
	assert(points.size() > 2);

	if (points.size() == 3) {
		//triangles are a simple (and dangerous) enough special case that we don't need all the extra data structure and processing baggage.
		mSimple = true;
		return;
	}

	//first get the leftmost and rightmost points
	Point left = points.first(), right = points.first();

	for (QList<Point>::const_iterator i = points.begin(); i < points.end(); ++i) {
		if (i->x < left.x) left = *i;
		else if (i->x == left.x) left.y = (left.y + i->y) / 2;
		else if (i->x > right.x) right = *i;
		else if (i->x == right.x) right.y = (right.y + i->y) / 2;
	}

	//if the line between these points is already an edge, we can't do the fast point checking.
	for (unsigned int i = 0; i < points.size(); ++i) {
		if (edge(i) == Segment(left, right)) {
			mSimple = true;
			return;
		}
	}

	//draw a line between these points
	double m = (right.y - left.y) / (right.x - left.x); //slope
	double c = left.y - m*left.x;

	//add each edge to the upper or lower set based on whether its points are above or below this line; both if either is on the line
	for (int i = 0; i < points.size(); ++i) {
		Segment e = edge(i);
		//swap points so they're in order, if needed
		if (e.a.x > e.b.x || (e.a.x == e.b.x && e.a.y > e.b.y)) std::swap(e.a, e.b);

		double aLineY = m*edge(i).a.x + c;
		double bLineY = m*edge(i).b.x + c;

		if (e.a.y <= aLineY || e.b.y <= bLineY)
			mPointsLower.insert(e);
		if (e.a.y >= aLineY || e.b.y >= bLineY)
			mPointsUpper.insert(e);
	}

	assert (mPointsUpper.size() + mPointsLower.size() > 2);
	removeCommonPoints<std::less<double> >(mPointsUpper);
	removeCommonPoints<std::greater<double> >(mPointsLower);
	assert (mPointsUpper.size() + mPointsLower.size() > 2);
}

//makes sure there aren't edges that have identical left or identical right points by picking one to remove (according to Op).
template<class Op>
void ConvexPolygon::removeCommonPoints(EdgeSet& points)
{
	Op op;

	//The only places these common points can actually occur are at the ends of the set.

	//First check the left end
	EdgeSet::iterator first = points.begin();
	EdgeSet::iterator second = first; ++second;
	//there's a chance that the point conflicting with the first point might not be the second in the array,
	//due to sorting; we have to search for it
	for (; second != points.end(); ++second) {
		if (first->a == second->a) {
			//purge the one with the higher/lower right point
			if (op(first->b.y, second->b.y)) points.erase(first);
			else points.erase(second);
			break;
		}
	}

	//now check the right side
	second = points.end(); --second;
	first = second; --first;
	if (first->b == second->b) {
		//purge the one with the higher/lower left point
		if (op(first->a.y, second->a.y)) points.erase(first);
		else points.erase(second);
	}
}

//assumes p.x is within the range of points
inline ConvexPolygon::Result ConvexPolygon::rayTest(Point p, const EdgeSet& points)
{
	EdgeSet::iterator e;
	Point a, b;
	Segment pDummy(Point(0,0), p); //dummy segment for searching
	e = points.upper_bound(pDummy); //find first point whose x is > p.x
	if (e == points.end()) {
		//we are either AT or DIRECTLY ABOVE the rightmost point
		--e; b = e->b; //rightmost point
		if (b.y == p.y) return in;
		else return noCross; //can't be an intersect
	}


	//check for being on the (non-vertical) line
	a = e->a; b = e->b;
	if (a.x < p.x && p.x < b.x) {
		double t = (p.x - b.x) / (a.x - b.x);
		double crossY = t*a.y + (1-t)*b.y;
		if (p.y == crossY) return in;
		else if (p.y > crossY) return cross;
	}

	else if (p.x == a.x) {
		//check for being on a vertical line
		if (b.x == p.x) {
			if (a.y <= p.y && p.y <= b.y) return in;
			else return out; //can't be in the polygon
		}

		//p.x == a.x; no vertical line; check for on or above a
		if (a.y == p.y) return in;
		else if (a.y < p.y) {
			//iff this is (one of) the leftmost point(s), it could be that the ray just "grazes" the polygon and we should return noCross
			if (a.x == points.begin()->a.x) return noCross;
			else return cross;
		}
	}

	//if it's in line with b, it has to be below it; no intersection

	/*else { //p.x == b.x
		if (p.y == b.y) return 1;
		else if (b.y < p.y) ray = -2; //guaranteed, i think.
	}*/

	return noCross;
}

//Returns whether the point p is inside the polygon (including the perimeter)
bool ConvexPolygon::contains(Point p) const
{
	if (mSimple) return simpleContains(p);

	//Since it is a convex polygon, a ray dropped from the point will cross at most 2 times with the perimeter -
	//once each on the upper portion and lower portion. If this ray crosses once, the point is inside the polygon;
	//if it crosses twice or not at all then it is outside (Jordan curve theorem).

	//CHECK FOR LEFT OF LEFT, RIGHT OF RIGHT
	EdgeSet::iterator right = mPointsUpper.end(); --right;
	if (p.x < mPointsUpper.begin()->a.x || p.x > right->b.x) return false;

	Result upper = rayTest(p, mPointsUpper);
	if (upper == in) return true;
	if (upper == out) return false;

	Result lower = rayTest(p, mPointsLower);
	if (lower == in) return true;
	if (lower == out) return false;

	//upper and lower both are either cross or noCross; need exactly 1 cross for it to be inside
	return upper != lower;
}

bool ConvexPolygon::simpleContains(Point p) const
{
	//just check if it's on the right side of each point
	for (unsigned int i = 0; i < points().size(); ++i) {
		Segment::PointRelation result = edge(i).classifyPoint(p);
		if (result == Segment::on) return true;
		if (result != Segment::right) return false;
	}

	return true;
}

}
