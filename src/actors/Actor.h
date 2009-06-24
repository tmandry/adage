#ifndef ACTOR_H_
#define ACTOR_H_

#include <string>
#include <vector>
#include <algorithm>
#include "world/MovingEntity.h"

class SteeringBehavior;

class Actor : public MovingEntity
{
public:
	Actor(Pointer<Entity> parent, std::string name="Actor");
	virtual ~Actor() {}

	int health() const { return mHealth; }

	ConstEntityList<Actor> neighbors();

	void addSteeringBehavior(SteeringBehavior* s);
	void remSteeringBehavior(SteeringBehavior* s);

protected:
	void setHealth(int health) { mHealth = health; }

	//Sets the radius used to search for neighbors.
	void setNeighborRadius(double radius) { mNeighborRadius = radius; }
	//Specifies which type of Actors qualifies as a neighbor - this class MUST inherit from Actor.
	void setNeighborType(std::string type) { mNeighborType = type; }

	virtual void updateEvent(double secsElapsed);

private:
	typedef std::vector<SteeringBehavior*> SBList;

	int mHealth;
	SBList mSteering;

	double mNeighborRadius;
	std::string mNeighborType;
	ConstEntityList<Actor> mNeighbors;
	bool mNeighborListValid;

	std::vector<Math::Vector> mHeadingList;
	unsigned int mHeadingIt;
	bool mHeadingListFull;
};

#endif /*ACTOR_H_*/
