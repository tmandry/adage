#ifndef SEEK_H_
#define SEEK_H_

#include "SteeringBehavior.h"
#include "math/Vector.h"
#include "math/Point.h"

class Seek : public SteeringBehavior
{
public:
	Seek(Math::Point target, Actor* parent);
	virtual ~Seek() {}
	
	Math::Vector calculate();
	
private:
	Math::Point mTarget;
};

#endif /*SEEK_H_*/
