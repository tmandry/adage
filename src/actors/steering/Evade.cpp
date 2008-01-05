#include "Evade.h"

Evade::Evade(Actor* parent, const MovingEntity* target)
	:	SteeringBehavior(parent, 1.0), /*weight 0.1?*/
		mTarget(target),
		mFlee(parent)
{
}

Math::Vector Evade::calculate()
{
	Math::Vector toTarget = mTarget->pos() - parent()->pos();
	
	//affects how far to look ahead
	static const double lookAheadFactor = 1.0;
	
	//lookahead time is proportional to the distance to the target,
	//and inversely proportional to the sum of the agents' speeds
	double lookAhead =
		lookAheadFactor * toTarget.length() /
		(parent()->maxSpeed() + mTarget->velocity().length());
	
	//now flee from predicted position
	mFlee.setTarget(mTarget->pos() + mTarget->velocity() * lookAhead);
	return mFlee.calculate();
}
