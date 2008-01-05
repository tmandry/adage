#ifndef GHOST_H_
#define GHOST_H_

#include "Actor.h"
#include "steering/Wander.h"

class Ghost : public Actor
{
public:
	Ghost(Math::Point pos, Entity* parent, std::string name="Ghost");
	virtual ~Ghost() {}

private:
	Wander mWander;
};

#endif /*GHOST_H_*/
