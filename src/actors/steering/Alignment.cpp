/*
 * Alignment.cpp
 *
 *  Created on: Jun 23, 2009
 *      Author: tyler
 */

#include "Alignment.h"

Alignment::Alignment(Pointer<Actor> parent)
	:	SteeringBehavior(parent, 6.0, 40)
{
}

Alignment::~Alignment()
{
}

Math::Vector Alignment::calculate()
{
	Math::Vector avgHeading(0,0);
	int neighborCount;

	EntityList<Actor> neighbors = parent()->neighbors();
	for (unsigned int i = 0; i < neighbors.size(); ++i) {
		//if (neighbors[i].pointer() == parent().pointer()) continue;
		if (!neighbors[i]) continue;

		avgHeading += neighbors[i]->heading();
		++neighborCount;
	}

	if (neighborCount == 0) return Math::Vector(0,0);

	avgHeading /= neighborCount;
	return (avgHeading - parent()->heading());
}
