#ifndef TRIGGERREGION_H_
#define TRIGGERREGION_H_

#include "math/Point.h"
#include "actors/Actor.h"

class TriggerRegion {
public:
	TriggerRegion() {}
	virtual ~TriggerRegion() {}

	virtual EntityList<Entity> search(Pointer<World> world) const = 0;
	virtual bool isTouching(Math::Point pos, double radius) const = 0;
};

class TriggerRegionCircle : public TriggerRegion {
public:
	TriggerRegionCircle(Math::Point pos, double radius): mPos(pos), mRadius(radius) {}

	EntityList<Entity> search(Pointer<World> world) const
	{
		return world->findEntities<Actor>(mPos, mRadius);
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
