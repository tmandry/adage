#include "MovingEntity.h"

MovingEntity::MovingEntity(Entity* parent, std::string name)
	:	Entity(parent,name), mMass(1), mMaxSpeed(9.0), mMaxForce(27.0)
{ subclass("MovingEntity"); }

void MovingEntity::updateEvent(double secsElapsed)
{
	//TODO: enforce max speed & max force?
	
	mForce.truncate(mMaxForce);
	Math::Vector accel = mForce / mMass;
	
	mVelocity += accel * secsElapsed;
	mVelocity.truncate(mMaxSpeed);
	
	setPos(pos() + (mVelocity*secsElapsed));
}
