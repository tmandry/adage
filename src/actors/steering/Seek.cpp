#include "Seek.h"
#include <iostream>

Seek::Seek(Pointer<Actor> parent, Math::Point target)
	:	SteeringBehavior(parent, 1.0, 80),
		mTarget(target)
{
}

Math::Vector Seek::calculate()
{
	Math::Vector desiredVelocity = (mTarget - parent()->pos()).normal() * parent()->maxSpeed();

	return desiredVelocity - parent()->velocity();
}
