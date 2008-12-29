#ifndef PURSUE_H_
#define PURSUE_H_

#include "SteeringBehavior.h"
#include "Seek.h"

class Pursue : public SteeringBehavior
{
public:
	Pursue(Pointer<Actor> parent, const Pointer<MovingEntity> target=Pointer<MovingEntity>());
	virtual ~Pursue() {}

	void setTarget(const Pointer<const MovingEntity>& target) { mTarget = target; }

	Math::Vector calculate();

private:
	friend class PersonView;
	Pointer<const MovingEntity> mTarget;
	Seek mSeek;
};

#endif /*PURSUE_H_*/
