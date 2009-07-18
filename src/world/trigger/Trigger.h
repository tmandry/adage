/*
 * Trigger.h
 *
 *  Created on: Jun 25, 2009
 *      Author: tyler
 */

#ifndef TRIGGER_H_
#define TRIGGER_H_

#include "world/Entity.h"
#include "TriggerRegion.h"

class Trigger: public Entity {
	ENTITY(Trigger)
public:
	Trigger(Pointer<Entity> parent, QString name = "Trigger"): Entity(parent, name), mActive(true), mRegion(0) { subclass(); setMovable(false); }
	virtual ~Trigger() { assert(mRegion); delete mRegion; }

	bool active() const { return mActive; }
	TriggerRegion* triggerRegion() const { return mRegion; }

	virtual void act(Pointer<Entity> target) = 0;

	virtual void updateEvent(double /*secsElapsed*/)
	{
		EntityList<Entity> targets = triggerRegion()->search(world());
		for (unsigned int i = 0; i < targets.size(); ++i)
			this->act(targets[i]);
	}

protected:
	void setActive(bool active) { mActive = active; }
	void setTriggerRegion(TriggerRegion* region) { if (mRegion) delete mRegion; mRegion = region; }

private:
	bool mActive;
	TriggerRegion* mRegion;
};

#endif /* TRIGGER_H_ */
