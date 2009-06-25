#include <cassert>
#include "Pursue.h"

Pursue::Pursue(Pointer<Actor> parent, const Pointer<MovingEntity> target)
	:	SteeringBehavior(parent, 0.9, 90),
		mTarget(target),
		mSeek(parent)
{
}

Math::Vector Pursue::calculate()
{
	assert(mTarget);

	Math::Vector toTarget = mTarget->pos() - parent()->pos();

	//if the target is ahead and almost directly facing the agent,
	//we can seek directly to its position
	if (
			toTarget.dot(parent()->heading()) > 0 &&	//ahead
			parent()->heading().dot(mTarget->heading()) < -0.95	//no more than 18 degrees difference
	) {
		mSeek.setTarget(mTarget->pos());
		return mSeek.calculate();
	}

	//otherwise, we need to predict where they're going to be

	//affects how far to look ahead
	static const double lookAheadFactor = 1.0;

	//lookahead time is proportional to the distance to the target,
	//and inversely proportional to the sum of the agents' speeds
	double lookAhead =
		lookAheadFactor * toTarget.length() /
		(parent()->maxSpeed() + mTarget->velocity().length());

	//now seek to predicted future position
	mSeek.setTarget(mTarget->pos() + mTarget->velocity()*lookAhead);
	return mSeek.calculate();
}
