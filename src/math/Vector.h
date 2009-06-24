// vector2d.h
//  Defines the Vector struct

#ifndef VECTOR_H_
#define VECTOR_H_

#include <cmath>
#include <limits>
#include <cassert>

#include "math/const.h"
#include "math/real.h"
#include "math/Point.h"

namespace Math {

struct Vector {
	double x,y;

	Vector(): x(0),y(0) {}
	Vector(double X, double Y): x(X),y(Y) {}
	explicit Vector(const Point& p): x(p.x),y(p.y) {} //convert from Point to Vector
	virtual ~Vector() {}

	//zero the vector
	void zero() {x = 0; y = 0;}

	//test if the vector is set to zero
	bool isZero() const {return ((x*x + y*y) < minDouble);}
	bool isNull() const {return x==0.0 && y==0.0;}

	//get the magnitude of the vector
	double length() const {return sqrt(x*x + y*y);}

	//get the magnitude squared of the vector (eliminating the square root)
	double lengthSq() const {return x*x + y*y;}

	//test if the vector is a unit vector
	bool isUnit() const {return isEqual(lengthSq(), 1);}

	//normalize the vector (set the magnitude to 1)
	void normalize()
	{
		double mag = length();
		if (mag > epsilon) {
				x /= mag;
				y /= mag;
		}
	}

	//return normal copy of this vector
	Vector normal() const
	{
		Vector norm(x,y);
		norm.normalize();
		return norm;
	}

	//calculate the dot product of this and another vector which is the cosine
	//of the angle between them
	double dot(const Vector& v) const {return x * v.x + y * v.y;}

	//calculate the angle between this and another vector, assuming both are unit vectors
	double angle(const Vector& v) const {assert(isUnit() && v.isUnit()); return acos(x * v.x + y * v.y);}

	//calculate absolute angle (from y-axis)
	double absAngle() const
	{
		double a = this->angle(Vector(1,0));
		if (y > 0) a = 2*PI - a;
		return a;
	}

	//returns a vector perpendicular to this - for convenience, when it doesn't matter
	Vector perp() const {return perpCW();}

	//returns the vector that is perpendicular to and directly clockwise of this
	Vector perpCW() const {return Vector(y, -x);}

	//returns the vector that is perpendicular to and directly counter-clockwise of this
	Vector perpCCW() const {return Vector(-y, x);}

	Vector iperpCW() const {return Vector(-y, x);}
	Vector iperpCCW() const {return Vector(y, -x);}

	//truncate the magnitude of the vector if it is too high
	void truncate(double max)
	{
		if (lengthSq() > max*max)
		{
			normalize();
			*this *= max;
		}
	}

	//convert from Vector to Point
	operator Point() const
	{
		return Point(x,y);
	}

	const Vector& operator+= (const Vector& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	const Vector& operator-= (const Vector& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	const Vector& operator*= (const double& rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	const Vector& operator/= (const double& rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
};

inline Point::Point(const Vector& v): x(v.x),y(v.y) {}

inline bool operator== (const Vector& lhs, const Vector& rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

inline bool operator!= (const Vector& lhs, const Vector& rhs)
{
	return (lhs.x != rhs.x) || (lhs.y != rhs.y);
}

inline bool operator< (const Vector& lhs, const Vector& rhs)
{
    return lhs.lengthSq() < rhs.lengthSq();
}

inline bool operator> (const Vector& lhs, const Vector& rhs)
{
    return lhs.lengthSq() > rhs.lengthSq();
}

inline const Vector operator- (const Vector& rhs)
{
	return Vector(-rhs.x, -rhs.y);
}

inline const Vector operator+ (const Vector& lhs, const Vector& rhs)
{
	Vector ret(lhs);
	ret += rhs;
	return ret;
}

inline const Vector operator- (const Vector& lhs, const Vector& rhs)
{
	Vector ret(lhs);
	ret -= rhs;
	return ret;
}

inline const Vector operator* (const Vector& lhs, const double& rhs)
{
	Vector ret(lhs);
	ret *= rhs;
	return ret;
}

inline const Vector operator/ (const Vector& lhs, const double& rhs)
{
	Vector ret(lhs);
	ret /= rhs;
	return ret;
}

inline double slope(const Vector& v)
{
	return v.y / v.x;
}

inline Point operator+ (const Point& lhs, const Vector& rhs)
{
	return Point(lhs.x+rhs.x, lhs.y+rhs.y);
}

inline Point operator- (const Point& lhs, const Vector& rhs)
{
	return Point(lhs.x-rhs.x, lhs.y-rhs.y);
}

inline Vector operator- (const Point& lhs, const Point& rhs)
{
	return Vector(lhs.x-rhs.x, lhs.y-rhs.y);
}

} // namespace Math

#endif /*VECTOR_H_*/
