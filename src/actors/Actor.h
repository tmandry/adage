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

	//do not call directly
	void addSteeringBehavior(SteeringBehavior* s);
	void remSteeringBehavior(SteeringBehavior* s);

protected:
	void setHealth(int health) { mHealth = health; }

	virtual void updateEvent(double secsElapsed);

private:
	typedef std::vector<SteeringBehavior*> SBList;

	int mHealth;
	SBList mSteering;

	std::vector<Math::Vector> mHeadingList;
	unsigned int mHeadingIt;
	bool mHeadingListFull;
};

#endif /*ACTOR_H_*/
