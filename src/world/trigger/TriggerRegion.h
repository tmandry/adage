/*
 * TriggerRegion.h
 *
 *  Created on: Jun 25, 2009
 *      Author: tyler
 */

#ifndef TRIGGERREGION_H_
#define TRIGGERREGION_H_

#include "math/Point.h"

class TriggerRegion {
public:
	TriggerRegion() {}
	virtual ~TriggerRegion() {}

	virtual ConstEntityList<Entity> search(Pointer<World> world) const = 0;
	virtual bool isTouching(Math::Point pos, double radius) const = 0;
};

class TriggerRegionCircle : public TriggerRegion {
public:
	TriggerRegionCircle(Math::Point pos, double radius): mPos(pos), mRadius(radius) {}

	ConstEntityList<Entity> search(Pointer<World> world) const
	{
		return world->findEntities<Entity>(mPos, mRadius, "Actor");
	}

	bool isTouching(Math::Point pos, double radius) const
	{
		return Math::distanceSq(pos, mPos) < (radius+mRadius)*(radius*mRadius);
	}

private:
	Math::Point mPos;
	double mRadius;
};

#endif /* TRIGGERREGION_H_ */
