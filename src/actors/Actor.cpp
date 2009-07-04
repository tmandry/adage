#include "Actor.h"
#include "steering/SteeringBehavior.h"
#include "math/Vector.h"

Actor::Actor(Pointer<Entity> parent, std::string name)
	:	MovingEntity(parent, name),
		mNeighborRadius(20.0),
		mNeighborType("Actor"),
		mNeighborListValid(false),
		mHeadingList(6, Math::Vector(0,0)),
		mHeadingIt(0),
		mHeadingListFull(false)
{
	subclass("Actor");
}

//Returns the neighbor list, meant to optimize steering behaviors that use this by only performing a search once per update
ConstEntityList<Actor> Actor::neighbors()
{
	if (mNeighborListValid) return mNeighbors;

	mNeighbors = world()->findEntities<Actor>(pos(), mNeighborRadius, mNeighborType);
	mNeighborListValid = true;
	return mNeighbors;
}

void Actor::addSteeringBehavior(SteeringBehavior* s)
{
	//insert s at the last possible position without breaking the sorting
	mSteering.insert(
		std::upper_bound(mSteering.begin(), mSteering.end(), s, SteeringBehavior::Sort()),
		s
	);
}

void Actor::remSteeringBehavior(SteeringBehavior* s)
{
	mSteering.erase(
		std::find(mSteering.begin(), mSteering.end(), s)
	);
}

void Actor::updateEvent(double secsElapsed)
{
	mNeighborListValid = false;

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

	//only update heading if traveling at a reasonable speed
	if (velocity().lengthSq() > 4.0) {
		Math::Vector heading = velocity().normal();
		assert(heading.x != 0 || heading.y != 0);

		mHeadingList[mHeadingIt] = heading;
		if (++mHeadingIt >= mHeadingList.size()) { mHeadingIt = 0; mHeadingListFull = true; }

		if (mHeadingListFull) {
			Math::Vector smoothedHeading(0,0);
			for (unsigned int i = 0; i < mHeadingList.size(); ++i) smoothedHeading += mHeadingList[i];
			smoothedHeading.normalize();

			setHeading(smoothedHeading);
		} else
			setHeading(heading);
	}
}
