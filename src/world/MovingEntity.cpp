#include "MovingEntity.h"

void MovingEntity::updateEvent(double secsElapsed)
{
	setPos(pos() + (mVelocity*secsElapsed));
}
