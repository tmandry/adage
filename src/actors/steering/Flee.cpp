#include "Flee.h"

Flee::Flee(Actor* parent, Math::Point target, double maxDistance)
	:	SteeringBehavior(parent, 1.0),
		mTarget(target),
		mMaxDistance(maxDistance)
{
}

Math::Vector Flee::calculate()
{
	//if target is too far away, do nothing
	if (distanceSq(parent()->pos(), mTarget) > mMaxDistance*mMaxDistance)
		return Math::Vector(0,0);
	
	Math::Vector desiredVelocity = (parent()->pos() - mTarget).normal() * parent()->maxSpeed();
	return desiredVelocity - parent()->velocity();
}
