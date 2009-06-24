#ifndef EVADE_H_
#define EVADE_H_

#include "SteeringBehavior.h"

class Evade : public SteeringBehavior
{
public:
	Evade(Pointer<Actor> parent, const Pointer<MovingEntity> target=Pointer<MovingEntity>(), double maxDistance=100.0);
	virtual ~Evade() {}

	void setTarget(Pointer<MovingEntity> target) { mTarget = target; }
	Pointer<MovingEntity> target() const { return mTarget; }

	Math::Vector calculate();

private:
	Pointer<MovingEntity> mTarget;
	double mMaxDistance;
};

#endif /*EVADE_H_*/
