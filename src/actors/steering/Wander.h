#ifndef WANDER_H_
#define WANDER_H_

#include "SteeringBehavior.h"

class Wander : public SteeringBehavior
{
public:
	Wander(Pointer<Actor> parent, double factor=1.0);
	virtual ~Wander() {}

	Math::Vector calculate();


	Math::Vector mWanderTarget;
private:
	//parameters of the wander behavior
	double mJitter;
	double mRadius;
	double mDistance;
};

#endif /*WANDER_H_*/
