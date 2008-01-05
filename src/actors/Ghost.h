#ifndef GHOST_H_
#define GHOST_H_

#include "Actor.h"
#include "steering/Wander.h"
#include "steering/Pursue.h"

class Ghost : public Actor
{
public:
	Ghost(Math::Point pos, Entity* parent, std::string name="Ghost");
	virtual ~Ghost() {}

private:
	friend class PersonView;
	
	Pursue mPursue;
	Wander mWander;
};

#endif /*GHOST_H_*/
