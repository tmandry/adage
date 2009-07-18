/*
 * Navigator.cpp
 *
 *  Created on: Jul 3, 2009
 *      Author: tyler
 */

#include "Navigator.h"
#include "steering/Seek.h"
#include "Person.h"
#include "nav/NavSystem.h"

Navigator::Navigator(Pointer<Entity> parent, QString name)
	:	Actor(parent, name)
{
	subclass();

	mSeek = new Seek(pointer());
	mSeek->off();
	addSteeringBehavior(mSeek);

	setMaxSpeed(20.0);

	setView(new PersonView(pointer(), QColor(46, 210, 255)));
	setVisible(true);
}

Navigator::~Navigator()
{
	if (mSeek) delete mSeek;
}

bool Navigator::goTo(Math::Point dest)
{
	/*Math::Point intersect;
	int side;
	NavSystem::NodeIterator node = world()->navSystem()->nodesBegin(); ++node;
	cout << NavNode::endsInside <<" "<< NavNode::exits <<" "<< NavNode::none << endl;;
	cout << node->classifyLine(Math::Segment(pos(), dest), side, intersect) << endl;*/

	if (world()->findPath(mPath, pos(), dest)) {
		mEdge = -1;
		nextEdge();
		return true;
	}
	else return false;
}

void Navigator::updateEvent(double secsElapsed)
{
	if (mSeek->isOn()) {
		//test to see if we've arrived at the waypoint
		if (distanceSq(pos(), mSeek->target()) < 1.0)
			nextEdge();
	}

	Actor::updateEvent(secsElapsed);
}

void Navigator::nextEdge()
{
	++mEdge;

	if (mEdge < mPath.edges.size()) {
		mSeek->setTarget(mPath.edges[mEdge].end);
		mSeek->on();
	} else {
		//if we're one over the last waypoint, we need to navigate to the final destination
		if (mEdge == mPath.edges.size()) {
			mSeek->setTarget(mPath.end);
			mSeek->on();
		} else { //otherwise, we've already reached that and we're done
			mSeek->off();
			setVelocity(Math::Vector(0,0));
		}
	}
}
