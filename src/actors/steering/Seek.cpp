#include "Seek.h"
#include <iostream>

Seek::Seek(Actor* parent, Math::Point target)
	:	SteeringBehavior(parent, 1.0),
		mTarget(target)
{
}

Math::Vector Seek::calculate()
{
	Math::Vector desiredVelocity = (mTarget - parent()->pos()).normal() * parent()->maxSpeed();
		
	return desiredVelocity - parent()->velocity();
}
