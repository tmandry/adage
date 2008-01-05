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
	Actor(Entity* parent, std::string name="Actor"): MovingEntity(parent, name) { subclass("Actor"); }
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
};

#endif /*ACTOR_H_*/
