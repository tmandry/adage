#ifndef FLEE_H_
#define FLEE_H_

#include "SteeringBehavior.h"
#include "math/Point.h"

class Flee : public SteeringBehavior
{
public:
	Flee(Actor* parent, Math::Point target, double maxDistance=100.0);
	virtual ~Flee() {}
	
	Math::Vector calculate();
	
	void setTarget(Math::Point target) { mTarget = target; }
	
private:
	Math::Point mTarget;
	double mMaxDistance;
};

#endif /*FLEE_H_*/
