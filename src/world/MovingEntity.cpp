#include "MovingEntity.h"

MovingEntity::MovingEntity(Entity* parent, std::string name)
	:	Entity(parent,name), mMass(640000), mMaxSpeed(3.0), mMaxForce(3.0)
{ subclass("MovingEntity"); }

void MovingEntity::updateEvent(double secsElapsed)
{
	//TODO: enforce max speed & max force?
	
	Math::Vector accel = mForce / mMass;
	
	mVelocity += accel * secsElapsed;
	mVelocity.truncate(mMaxSpeed);
	
	setPos(pos() + (mVelocity*secsElapsed));
}
