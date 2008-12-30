#include "Ghost.h"
#include "Person.h"
#include "world/World.h"
#include "math/rand.h"

Ghost::Ghost(Math::Point pos, Pointer<Entity> parent, std::string name)
	:	Actor(parent, name)
{
	subclass("Ghost");

	setPos(pos);
	setMaxSpeed(9.0);

	mPursue = new Pursue(pointer());
	mWander = new Wander(pointer());
	addSteeringBehavior(mPursue);
	addSteeringBehavior(mWander);

	PersonView* view = new PersonView(pointer());
	view->setColor(Qt::green); // HULK SMASH!  -- 20070108 iank
	setView(view);
	setVisible(true);

	newTarget();
}

void Ghost::updateEvent(double secsElapsed)
{
	if (!mTarget) newTarget();

	//check if we caught them
	if (mTarget && distanceSq(pos(), mTarget->pos()) < 3.0) {
		mTarget->remove();

		Ghost* victim = new Ghost(mTarget->pos(), world());
		victim->setVelocity(mTarget->velocity());

		newTarget();
	}

	newTargetTimer += secsElapsed;
	if (newTargetTimer > 30) newTarget();

	assert(mTarget || !(mPursue->isOn()));
	Actor::updateEvent(secsElapsed);
}

void Ghost::newTarget()
{
	//pick a random person to pursue
	ConstEntityList<Person> people = world()->findEntities<Person>("Person");

	if (!people.empty()) {
		int idx = Math::randInt(0, people.size()-1);
		mTarget = people[idx];
		mPursue->setTarget(mTarget);
		newTargetTimer = 0;
	} else {
		mTarget.release();
		mPursue->off();
	}
}
