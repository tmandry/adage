#ifndef ARRIVE_H_
#define ARRIVE_H_

#include "SteeringBehavior.h"

class Arrive : public SteeringBehavior
{
public:
	enum Deceleration { slow = 3, normal = 2, fast = 1 };

	Arrive(Actor* parent, Math::Point target, Deceleration decel = normal)
		:	SteeringBehavior(parent, 1.0),
			mTarget(target),
			mDecel(decel)
	{}
	virtual ~Arrive() {}
	
	Math::Vector calculate();
	
private:
	Math::Point mTarget;
	Deceleration mDecel;
};

#endif /*ARRIVE_H_*/
