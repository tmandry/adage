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
#include "world/GhostBustersHQ.h"
#include "world/World.h"
#include "world/GhostTrap.h"
#include "math/rand.h"
#include "steering/AvoidWalls.h"
#include "steering/Separation.h"

GhostBuster::GhostBuster(Math::Point pos, Pointer<Entity> parent, std::string name)
	:	Actor(parent, name),
		mKillCount(0)
{
	subclass("GhostBuster");

	ConstEntityList<GhostBustersHQ> hq = world()->findEntities<GhostBustersHQ>("GhostBustersHQ");
	if (hq.size() != 0) mHQ = hq[0];

	setPos(pos);
	setMaxSpeed(10.0);

	mWander = new Wander(pointer());
	mPursue = new Pursue(pointer());
	addSteeringBehavior(new AvoidWalls(pointer()));
	addSteeringBehavior(mPursue);
	addSteeringBehavior(new Separation(pointer()));
	addSteeringBehavior(mWander);

	PersonView* view = new PersonView(pointer(), Qt::yellow);
	//view->setColor(Qt::yellow);
	setView(view);
	setVisible(true);

	if (mHQ){
		mHQ->newAgent(pointer());
		mHQ->reassign(pointer());
	}
}

GhostBuster::~GhostBuster() {
	// TODO Auto-generated destructor stub
}

void GhostBuster::updateEvent(double secsElapsed)
{
	if (!mTarget && mHQ) mHQ->reassign(pointer());

	if (mTarget && distanceSq(pos(), mTarget->pos()) < 25.0) {
		//GHOST BUSTERS!!!
		mTarget->remove();
		++mKillCount;

		if (mKillCount % 5 == 0) { //new comrade
			GhostBuster* comrade = new GhostBuster(mTarget->pos(), world());
			comrade->setVelocity(mTarget->velocity());
		}

		if ((mKillCount+1) % 4 == 0) { //new trap
			new GhostTrap(world(), pos());
			printComm("Ghost trap deployed.");
		}

		if (mHQ) {
			mHQ->targetCaught(pointer(), mTarget);
			//request reassignment from GBHQ
			mHQ->reassign(pointer());
		}
	}

	assert(mTarget || (!mPursue->isOn()));
	Actor::updateEvent(secsElapsed);
}

void GhostBuster::setTarget(Pointer<Ghost> target)
{
	mTarget = target;

	if (target) {
		mPursue->setTarget(target);
		mPursue->on();
	} else {
		mPursue->off();
	}
}
