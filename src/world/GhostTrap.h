/*
 * GhostTrap.h
 *
 *  Created on: Jun 25, 2009
 *      Author: tyler
 */

#ifndef GHOSTTRAP_H_
#define GHOSTTRAP_H_

#include "trigger/LimitedLifetimeTrigger.h"
#include "View.h"

class GhostTrap: public LimitedLifetimeTrigger {
	ENTITY(GhostTrap)
	AUTO_FACTORY
public:
	GhostTrap(Math::Point pos, Pointer<Entity> parent, QString name = "GhostTrap");
	virtual ~GhostTrap();

	void act(Pointer<Entity> target);
	void updateEvent(double secsElapsed);

private:
	void detonate();

	CircleView* mView;

	bool mDetonated;
	double mKillTimer;
	int mKillCount;
};

#endif /* GHOSTTRAP_H_ */
