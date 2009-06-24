#ifndef POINT_H_
#define POINT_H_

#include <cmath>
#include <QPointF>

#include "math/real.h"

namespace Math {

struct Vector;

struct Point
{
	double x,y;

	Point(): x(0), y(0) {}
	Point(double X, double Y): x(X),y(Y) {}
	explicit Point(const QPointF& p): x(p.x()),y(p.y()) {}
	explicit inline Point(const Vector& v); //definition in Vector.h
	Point(const Point& p): x(p.x), y(p.y) {}
	virtual ~Point() {}

	operator QPointF()
	{
		return QPointF(x,y);
	}

	/*const Point& operator+= (const Vector& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	const Point& operator-= (const Vector& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}*/
};

//return the distance between two points
inline double distance(const Point& a, const Point& b)
{
	double distX = a.x - b.x;
	double distY = a.y - b.y;

	return sqrt(distX*distX + distY*distY);
}

//return the distance between two points squared
inline double distanceSq(const Point& a, const Point& b)
{
	double distX = a.x - b.x;
	double distY = a.y - b.y;

	return distX*distX + distY*distY;
}

inline bool operator== (const Point& lhs, const Point& rhs)
{
	return isEqual(lhs.x, rhs.x) && isEqual(lhs.y, rhs.y);
}

inline bool operator!= (const Point& lhs, const Point& rhs)
{
	return !(isEqual(lhs.x, rhs.x) && isEqual(lhs.y, rhs.y));
}

} //namespace Math

#endif /*POINT_H_*/
