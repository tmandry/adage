#ifndef WALL_H_
#define WALL_H_

#include <utility>
#include "math/Point.h"
#include "math/Vector.h"
#include "math/Segment.h"

class Wall
{
public:
	typedef std::pair<Math::Vector, Math::Vector> NormalPair;
	
private:
	Math::Point mA, mB;
	NormalPair mNormals;
	
	void calculateNormals()
	{
		Math::Vector temp = mA-mB;
		temp.normalize();
		
		mNormals.first = temp.perpCW();
		mNormals.second = temp.perpCCW();
	}

public:
	Wall(double x1,double y1, double x2,double y2): mA(x1,y1), mB(x2,y2) { calculateNormals(); }
	Wall(Math::Point a, Math::Point b): mA(a),mB(b) { calculateNormals(); }
	virtual ~Wall() {}
	
	Math::Segment segment() const
	{
		return Math::Segment(mA, mB);
	}
	
	//TODO: convenience functions that use existing functions to test for collision?
};

#endif /*WALL_H_*/
