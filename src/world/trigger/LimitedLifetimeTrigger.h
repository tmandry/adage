/*
 * LimitedLifetimeTrigger.h
 *
 *  Created on: Jun 25, 2009
 *      Author: tyler
 */

#ifndef LIMITEDLIFETIMETRIGGER_H_
#define LIMITEDLIFETIMETRIGGER_H_

#include "Trigger.h"

class LimitedLifetimeTrigger : public Trigger {
public:
	LimitedLifetimeTrigger(double lifetime): mLifetime(lifetime) { subclass("LimitedLifetimeTrigger"); }
	virtual ~LimitedLifetimeTrigger() {}

	virtual void act(Pointer<Entity> target) = 0;

	virtual void updateEvent(double secsElapsed)
	{
		mLifetime -= secsElapsed;
		if (mLifetime <= 0)
			remove();
	}

private:
	//remaining lifetime in seconds
	double mLifetime;
};

#endif /* LIMITEDLIFETIMETRIGGER_H_ */
