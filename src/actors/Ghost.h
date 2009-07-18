#ifndef GHOST_H_
#define GHOST_H_

#include "Pointer.h"
#include "Actor.h"
#include "Person.h"
#include "steering/Wander.h"
#include "steering/Pursue.h"

class Ghost : public Actor
{
	ENTITY(Ghost)
	AUTO_FACTORY
public:
	Ghost(Math::Point pos, Pointer<Entity> parent, QString name="Ghost");
	virtual ~Ghost() {}

private:
	friend class PersonView;

	virtual void updateEvent(double secsElapsed);
	double newTargetTimer;

	void newTarget();

	int mKillCount;

	Pursue* mPursue;
	Wander* mWander;
	Pointer<Person> mTarget;
};

#endif /*GHOST_H_*/
