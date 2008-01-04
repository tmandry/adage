#include "Actor.h"
#include "steering/SteeringBehavior.h"
#include "math/Vector.h"

void Actor::addSteeringBehavior(SteeringBehavior* s)
{
	//insert s at the last possible position without breaking the sorting
	mSteering.insert(
		std::upper_bound(mSteering.begin(), mSteering.end(), s, SteeringBehaviorSort()),
		s
	);
}

void Actor::delSteeringBehavior(SteeringBehavior* s)
{
	mSteering.erase(
		std::find(mSteering.begin(), mSteering.end(), s)
	);
}

void Actor::updateEvent(double secsElapsed)
{
	//combine steering forces: weighted truncated running sum (with prioritization)
	Math::Vector steeringForce;
	
	Math::Vector force;
	//for each steering behavior
	for (SBList::iterator i = mSteering.begin(); i < mSteering.end(); ++i) {
		if (!(*i)->isOn()) continue;
		
		//calculate how much steering force is being used, and how much is left over
		double magSoFar = steeringForce.length();
		double magRemaining = maxForce() - magSoFar;
		
		//if our vector has hit max, ignore any steering forces left
		if (magRemaining <= 0.0) break;
		
		//calculate force to add
		force = (*i)->calculate() * (*i)->factor() * 15.0;
		
		//truncate vector we're about to add with the magnitude remaining
		// NOTE: in reality the amount of magnitude about to be added depends (a lot) upon the direction, but this technique works anyway
		force.truncate(magRemaining);
		
		steeringForce += force;
	}
	
	setForce(steeringForce);
	MovingEntity::updateEvent(secsElapsed);
}
