#include <utility>
#include "Arrive.h"

Math::Vector Arrive::calculate()
{
	Math::Vector toTarget = mTarget - parent()->pos();
	double dist = toTarget.length();
	
	if (dist <= 0.0) return Math::Vector(0,0);
	
	//tweaks Deceleration because it's enumerated as an int
	const double decelFactor = 0.3;
	
	//calcuate speed we should be going at this point
	double speed = dist / (mDecel * decelFactor);
	speed = std::min(speed, parent()->maxSpeed());
	
	//normalize toTarget and use correct speed
	Math::Vector desiredVelocity = toTarget * speed / dist;
	
	return (desiredVelocity - parent()->velocity());
}
