/*
 * GhostBuster.cpp
 *
 *  Created on: Dec 27, 2008
 *      Author: tyler
 */

#include <cassert>
#include <iostream>
#include "GhostBuster.h"
#include "Person.h"
#include "world/World.h"
#include "math/rand.h"
#include "steering/AvoidWalls.h"

GhostBuster::GhostBuster(Math::Point pos, Pointer<Entity> parent, std::string name)
	:	Actor(parent, name),
		mWander(Pointer<Actor>::staticPointerCast(pointer())),
		mPursue(Pointer<Actor>::staticPointerCast(pointer()))
{
	subclass("GhostBuster");

	setPos(pos);
	setMaxSpeed(9.1);

	addSteeringBehavior(new AvoidWalls(Pointer<Actor>::staticPointerCast(pointer())));
	addSteeringBehavior(&mPursue);
	addSteeringBehavior(&mWander);

	PersonView* view = new PersonView(Pointer<Actor>::staticPointerCast(pointer()));
	view->setColor(Qt::yellow);
	setView(view);
	setVisible(true);

	newTarget();
}

GhostBuster::~GhostBuster() {
	// TODO Auto-generated destructor stub
}

void GhostBuster::updateEvent(double secsElapsed)
{
	if (!mTarget) newTarget();

	if (mTarget && distanceSq(pos(), mTarget->pos()) < 3.0) {
		mTarget->remove();
		newTarget();
	}

	Actor::updateEvent(secsElapsed);
}

void GhostBuster::newTarget()
{
	//pick a new random Ghost to pursue
	ConstEntityList<Ghost> ghosts = world()->findEntities<Ghost>("Ghost");
	if (!ghosts.empty()) {
		int idx = Math::randInt(0, ghosts.size()-1);
		mTarget = ghosts[idx];
		assert(mTarget);
		mPursue.setTarget(mTarget);
	} else {
		mTarget.release();
		mPursue.off();
	}
}
