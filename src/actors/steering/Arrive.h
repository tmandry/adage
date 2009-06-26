#ifndef ARRIVE_H_
#define ARRIVE_H_

#include "SteeringBehavior.h"

class Arrive : public SteeringBehavior
{
public:
	enum Deceleration { slow = 3, normal = 2, fast = 1 };

	Arrive(Pointer<Actor> parent, Math::Point target, Deceleration decel = normal, double multiplier = 1.0, int priority = 80)
		:	SteeringBehavior(parent, multiplier, priority),
			mTarget(target),
			mDecel(decel)
	{}
	virtual ~Arrive() {}

	void setTarget(Math::Point target) { mTarget = target; }
	Math::Point target() const { return mTarget; }

	Math::Vector calculate();

private:
	Math::Point mTarget;
	Deceleration mDecel;
};

#endif /*ARRIVE_H_*/
