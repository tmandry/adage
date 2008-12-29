#include <QMatrix>
#include "MovingEntity.h"
#include "math/convert.h"

MovingEntity::MovingEntity(Pointer<Entity> parent, std::string name)
	:	Entity(parent,name), mMass(1), mHeading(1,0), mMaxSpeed(9.0), mMaxForce(27.0)
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

Math::Point MovingEntity::toWorldSpace(Math::Point local) const
{
	QMatrix matrix;
	//matrix.translate(pos().x, pos().y);
	matrix.rotate( Math::toDegrees(heading().absAngle()) );
	return Math::Point( matrix.map(local) ) + Math::Vector(pos().x,pos().y);
}
