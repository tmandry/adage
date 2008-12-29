#include "Ghost.h"
#include "Person.h"
#include "world/World.h"
#include "math/rand.h"

Ghost::Ghost(Math::Point pos, Pointer<Entity> parent, std::string name)
	:	Actor(parent, name),
		mPursue(Pointer<Ghost>::staticPointerCast(pointer())),
		mWander(Pointer<Ghost>::staticPointerCast(pointer()))
{
	subclass("Ghost");

	setPos(pos);
	setMaxSpeed(9.0);

	addSteeringBehavior(&mPursue);
	addSteeringBehavior(&mWander);

	PersonView* view = new PersonView(Pointer<Ghost>::staticPointerCast(pointer()));
	view->setColor(Qt::green); // HULK SMASH!  -- 20070108 iank
	setView(view);
	setVisible(true);

	newTarget();
}

void Ghost::updateEvent(double secsElapsed)
{
	if (!mTarget) newTarget();

	//check if we caught them
	if (distanceSq(pos(), mTarget->pos()) < 3.0) {
		mTarget->remove();

		Ghost* victim = new Ghost(mTarget->pos(), world());
		victim->setVelocity(mTarget->velocity());

		newTarget();
	}

	newTargetTimer += secsElapsed;
	if (newTargetTimer > 10) newTarget();

	Actor::updateEvent(secsElapsed);
}

void Ghost::newTarget()
{
	//pick a random person to pursue
	ConstEntityList<Person> people = world()->findEntities<Person>("Person");
	int idx = Math::randInt(0, people.size());
	mTarget = people[idx];
	mPursue.setTarget(mTarget);
	newTargetTimer = 0;
}
