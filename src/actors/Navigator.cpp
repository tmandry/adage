/*
 * Navigator.cpp
 *
 *  Created on: Jul 3, 2009
 *      Author: tyler
 */

#include "Navigator.h"
#include "steering/Seek.h"
#include "Person.h"

Navigator::Navigator(Pointer<Entity> parent, std::string name)
	:	Actor(parent, name),
		mSeek(new Seek(pointer()))
{
	subclass("Navigator");

	mSeek->off();
	addSteeringBehavior(mSeek);

	setMaxSpeed(15.0);

	setView(new PersonView(pointer(), QColor(46, 210, 255)));
	setVisible(true);
}

Navigator::~Navigator()
{
	if (mSeek) delete mSeek;
}

#include <iostream>
using namespace std;

bool Navigator::goTo(Math::Point dest)
{
	cout << "*** " << dest.x << " " << dest.y << endl;

	if (world()->findPath(mPath, pos(), dest)) {
		mWaypoint = -1;
		nextWaypoint();
		return true;
	}
	else return false;
}

void Navigator::updateEvent(double secsElapsed)
{
	if (mSeek->isOn()) {
		//test to see if we've arrived at the waypoint
		if (distanceSq(pos(), mSeek->target()) < 1.0)
			nextWaypoint();
	}

	Actor::updateEvent(secsElapsed);
}

void Navigator::nextWaypoint()
{
	++mWaypoint;

	if (mWaypoint < mPath.waypoints.size()) {
		mSeek->setTarget(mPath.waypoints[mWaypoint].destination);
		mSeek->on();
	} else {
		//if we're one over the last waypoint, we need to navigate to the final destination
		if (mWaypoint == mPath.waypoints.size()) {
			mSeek->setTarget(mPath.end);
			mSeek->on();
		} else { //otherwise, we've already reached that and we're done
			mSeek->off();
			setVelocity(Math::Vector(0,0));
		}
	}
}
