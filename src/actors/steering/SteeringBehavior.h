#ifndef STEERINGBEHAVIOR_H_
#define STEERINGBEHAVIOR_H_

#include "actors/Actor.h"
#include "math/Vector.h"

class SteeringBehavior
{
public:
	SteeringBehavior(Actor* parent, double factor, bool on=true)
		:	mParent(parent), mFactor(factor), mOn(on)
	{
		/*mParent->addSteeringBehavior(this);*/
	}
	
	virtual ~SteeringBehavior()
	{
		/*mParent->delSteeringBehavior(this);*/
	}
	
	virtual Math::Vector calculate() = 0;
	
	void on() { mOn = true; }
	void off() { mOn = false; }
	bool isOn() const { return mOn; }
	
	double factor() const { return mFactor; }

protected:
	Actor* parent() const { return mParent; }

private:
	Actor* mParent;
	double mFactor;
	bool mOn;
};

class SteeringBehaviorSort
{
public:
    bool operator()(const SteeringBehavior* lhs, const SteeringBehavior* rhs)
    {
        return lhs->factor() > rhs->factor();
    }
};

#endif /*STEERINGBEHAVIOR_H_*/
