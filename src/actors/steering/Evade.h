#ifndef EVADE_H_
#define EVADE_H_

#include "SteeringBehavior.h"
#include "Flee.h"

class Evade : public SteeringBehavior
{
public:
	Evade(Actor* parent, const MovingEntity* target=0);
	virtual ~Evade() {}
	
	Math::Vector calculate();
	
private:
	const MovingEntity* mTarget;
	Flee mFlee;
};

#endif /*EVADE_H_*/
