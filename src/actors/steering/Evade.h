#ifndef EVADE_H_
#define EVADE_H_

#include "SteeringBehavior.h"
#include "Flee.h"

class Evade : public SteeringBehavior
{
public:
	Evade(Pointer<Actor> parent, const Pointer<MovingEntity> target=Pointer<MovingEntity>());
	virtual ~Evade() {}

	Math::Vector calculate();

private:
	const Pointer<MovingEntity> mTarget;
	Flee mFlee;
};

#endif /*EVADE_H_*/
