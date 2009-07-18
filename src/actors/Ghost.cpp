#include "Ghost.h"
#include "Person.h"
#include "world/World.h"
#include "math/rand.h"
#include "world/GhostPortal.h"

Ghost::Ghost(Math::Point pos, Pointer<Entity> parent, std::string name)
	:	Actor(parent, name),
		mKillCount(0)
{
	subclass();

	setPos(pos);
	setMaxSpeed(9.0);

	mPursue = new Pursue(pointer());
	mWander = new Wander(pointer());
	addSteeringBehavior(mPursue);
	addSteeringBehavior(mWander);

	PersonView* view = new PersonView(pointer(), Qt::green);
	//view->setColor(Qt::green); // HULK SMASH!  -- 20070108 iank
	setView(view);
	setVisible(true);

	newTarget();
}

void Ghost::updateEvent(double secsElapsed)
{
	if (!mTarget) newTarget();

	//check if we caught them
	if (mTarget && distanceSq(pos(), mTarget->pos()) < 9.0) {
		mTarget->remove();

		Ghost* victim = new Ghost(mTarget->pos(), world());
		victim->setVelocity(mTarget->velocity());

		++mKillCount;
		if (mKillCount % 3 == 0) new DormantGhostPortal(pos(), world());

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
	EntityList<Person> people = world()->findEntities<Person>(pos(), 35.0);
	if (people.empty() || Math::randFloat(0, 1) < .5) people = world()->findEntities<Person>();

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
