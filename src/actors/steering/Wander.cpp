#include "Wander.h"
#include "math/rand.h"

Wander::Wander(Pointer<Actor> parent, double factor)
	:	SteeringBehavior(parent, factor),
		mJitter(0.4),
		mRadius(1.0),
		mDistance(0.7)
{
}

Math::Vector Wander::calculate()
{
	//move target around a bit
	mWanderTarget += Math::Vector(
		Math::randFloat(-1.0, 1.0) * mJitter,
		Math::randFloat(-1.0, 1.0) * mJitter
	);

	//constrain it to wander circle
	mWanderTarget = mWanderTarget.normal() * mRadius;

	//move target (and circle) in front of us
	Math::Point targetLocal = Math::Point(mDistance,0) + mWanderTarget;
	//map to world space
	Math::Point targetWorld = parent()->toWorldSpace(targetLocal);

	return (targetWorld - parent()->pos()) * 1.5;
}
