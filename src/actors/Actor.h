#ifndef ACTOR_H_
#define ACTOR_H_

#include <string>
#include <QString>
#include <vector>
#include <algorithm>
#include "world/MovingEntity.h"

class SteeringBehavior;

class Actor : public MovingEntity
{
	ENTITY(Actor)
public:
	Actor(Pointer<Entity> parent, std::string name="Actor");
	virtual ~Actor() {}

	int health() const { return mHealth; }

	EntityList<Actor> neighbors();

	void addSteeringBehavior(SteeringBehavior* s);
	void remSteeringBehavior(SteeringBehavior* s);

protected:
	void setHealth(int health) { mHealth = health; }

	//Sets the radius used to search for neighbors.
	void setNeighborRadius(double radius) { mNeighborRadius = radius; }
	//Specifies which type of Actors qualifies as a neighbor - this class MUST inherit from Actor.
	template <class E>
	void setNeighborType() { mNeighborType = Entity::_className<E>(); }

	virtual void updateEvent(double secsElapsed);

private:
	typedef std::vector<SteeringBehavior*> SBList;

	int mHealth;
	SBList mSteering;

	double mUpdateStTimer;
	const double mUpdateStInterval;
	double mUpdateNeighborsTimer;
	const double mUpdateNeighborsInterval;

	double mNeighborRadius;
	QString mNeighborType;
	EntityList<Actor> mNeighbors;
	bool mNeighborListValid;

	std::vector<Math::Vector> mHeadingList;
	unsigned int mHeadingIt;
	bool mHeadingListFull;
};

#endif /*ACTOR_H_*/
