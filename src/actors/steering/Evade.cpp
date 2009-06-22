#include "Evade.h"

Evade::Evade(Pointer<Actor>  parent, const Pointer<MovingEntity> target, double maxDistance)
	:	SteeringBehavior(parent, 0.6), /*weight 0.1?*/
		mTarget(target),
		mMaxDistance(maxDistance)
{
}

Math::Vector Evade::calculate()
{
	//affects how far to look ahead
	static const double lookAheadFactor = 1.0;

	if (!mTarget) return Math::Vector();
	Math::Vector toTarget = mTarget->pos() - parent()->pos();

	//if target is too far away, do nothing
	if (toTarget.lengthSq() > mMaxDistance*mMaxDistance)
		return Math::Vector();

	double targetSpeed = mTarget->velocity().length();
	//lookahead time is proportional to the distance to the target,
	//and inversely proportional to the sum of the agents' speeds
	double lookAhead =
		lookAheadFactor * toTarget.lengthSq() /
		(parent()->maxSpeed() + targetSpeed)*(parent()->maxSpeed() + targetSpeed);

	//now flee from predicted position
	Math::Point aim = mTarget->pos() + mTarget->velocity() * lookAhead;
	Math::Vector desiredVelocity = (parent()->pos() - aim).normal() * parent()->maxSpeed();
	return desiredVelocity - parent()->velocity();
}
