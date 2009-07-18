/*
 * GhostPortal.h
 *
 *  Created on: Jun 25, 2009
 *      Author: tyler
 */

#ifndef GHOSTPORTAL_H_
#define GHOSTPORTAL_H_

#include "Entity.h"
#include "trigger/LimitedLifetimeTrigger.h"

//DormantGhostPortals are gray and lie waiting for an unsuspecting human to activate it, or they will die out.
class DormantGhostPortal: public LimitedLifetimeTrigger {
	ENTITY(DormantGhostPortal)
	AUTO_FACTORY
public:
	DormantGhostPortal(Math::Point pos, Pointer<Entity> parent);

	void act(Pointer<Entity> target);
};

//GhostPortals (active) are thick red circles and spawn a bunch of ghosts from the underworld.
class GhostPortal: public Entity {
	ENTITY(GhostPortal)
public:
	GhostPortal(Pointer<Entity> parent, Math::Point pos);
	virtual ~GhostPortal() {}

	void updateEvent(double secsElapsed);

private:
	double mTimer;
	int mGhostsToSpawn;
};

#endif /* GHOSTPORTAL_H_ */
