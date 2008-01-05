#ifndef PURSUE_H_
#define PURSUE_H_

#include "SteeringBehavior.h"
#include "Seek.h"

class Pursue : public SteeringBehavior
{
public:
	Pursue(Actor* parent, const MovingEntity* target=0);
	virtual ~Pursue() {}
	
	void setTarget(const MovingEntity* target) { mTarget = target; }
	
	Math::Vector calculate();
	
private:
	friend class PersonView;
	MovingEntity const* mTarget;
	Seek mSeek;
};

#endif /*PURSUE_H_*/
