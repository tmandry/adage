/*
 * GhostTrap.cpp
 *
 *  Created on: Jun 25, 2009
 *      Author: tyler
 */

#include "GhostTrap.h"

GhostTrap::GhostTrap(Pointer<Entity> parent, Math::Point pos, std::string name)
	:	Trigger(parent, name)
{
	setTriggerRegion(new TriggerRegionCircle(pos, 7));
	setView(new GhostTrapView(pos, 7));
	setVisible(true);
}

GhostTrap::~GhostTrap() {
}

void GhostTrap::act(Pointer<Entity> target)
{
	if (target->inherits("Ghost")) {
		printComm("Ghost has been caught by a trap and sent back into the Underworld!");

		target->remove();
		remove(); //traps only fire once...
	}
}

void GhostTrapView::paint(QPainter* p)
{
	p->save();

	p->setPen(QPen(Qt::darkGreen, 0.5));
	p->drawEllipse(mPos, mRadius, mRadius);

	p->restore();
}
