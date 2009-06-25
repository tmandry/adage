/*
 * GhostTrap.h
 *
 *  Created on: Jun 25, 2009
 *      Author: tyler
 */

#ifndef GHOSTTRAP_H_
#define GHOSTTRAP_H_

#include "trigger/Trigger.h"
#include "View.h"

class GhostTrap: public Trigger {
public:
	GhostTrap(Pointer<Entity> parent, Math::Point pos, std::string name = "GhostTrap");
	virtual ~GhostTrap();

	void act(Pointer<Entity> target);
};

class GhostTrapView : public View {
public:
	GhostTrapView(Math::Point pos, double radius): mPos(pos), mRadius(radius) {}

	void paint(QPainter* p);

private:
	Math::Point mPos;
	double mRadius;
};

#endif /* GHOSTTRAP_H_ */
