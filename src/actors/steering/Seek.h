#ifndef SEEK_H_
#define SEEK_H_

#include "SteeringBehavior.h"
#include "math/Vector.h"
#include "math/Point.h"

class Seek : public SteeringBehavior
{
public:
	Seek(Actor* parent, Math::Point target = Math::Point());
	virtual ~Seek() {}
	
	void setTarget(Math::Point target) { mTarget = target; }
	
	Math::Vector calculate();
	
private:
	Math::Point mTarget;
};

#endif /*SEEK_H_*/
