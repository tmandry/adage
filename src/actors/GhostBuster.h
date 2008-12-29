/*
 * GhostBuster.h
 *
 *  Created on: Dec 27, 2008
 *      Author: tyler
 */

#ifndef GHOSTBUSTER_H_
#define GHOSTBUSTER_H_

#include <string>
#include "Pointer.h"
#include "Actor.h"
#include "Person.h"
#include "Ghost.h"
#include "steering/Wander.h"
#include "steering/Pursue.h"

class GhostBuster : public Actor {
public:
	GhostBuster(Math::Point pos, Pointer<Entity> parent, std::string name = "GhostBuster");
	virtual ~GhostBuster();

private:
	friend class PersonView;

	virtual void updateEvent(double secsElapsed);

	void newTarget();

	Wander mWander;
	Pursue mPursue;
	Pointer<Ghost> mTarget;
};

#endif /* GHOSTBUSTER_H_ */
