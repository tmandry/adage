#ifndef STEERINGBEHAVIOR_H_
#define STEERINGBEHAVIOR_H_

#include "actors/Actor.h"
#include "math/Vector.h"

class SteeringBehavior
{
public:
	class Sort
	{
	public:
	    bool operator()(const SteeringBehavior* lhs, const SteeringBehavior* rhs)
	    {
	        return lhs->priority() > rhs->priority();
	    }
	};

	SteeringBehavior(Pointer<Actor> parent, double factor, int priority, bool on=true)
		:	mParent(parent), mFactor(factor), mPriority(priority), mOn(on)
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

	void setFactor(double factor) { mFactor = factor; }
	double factor() const { return mFactor; }
	int priority() const { return mPriority; }

protected:
	Pointer<Actor> parent() const { return mParent; }

private:
	Pointer<Actor> mParent;
	double mFactor;
	int mPriority;
	bool mOn;
};

#endif /*STEERINGBEHAVIOR_H_*/
