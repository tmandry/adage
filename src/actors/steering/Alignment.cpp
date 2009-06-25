/*
 * Alignment.cpp
 *
 *  Created on: Jun 23, 2009
 *      Author: tyler
 */

#include "Alignment.h"

Alignment::Alignment(Pointer<Actor> parent)
	:	SteeringBehavior(parent, 0.5, 40)
{
}

Alignment::~Alignment()
{
}

Math::Vector Alignment::calculate()
{
	Math::Vector avgHeading(0,0);
	int neighborCount;

	ConstEntityList<Actor> neighbors = parent()->neighbors();
	for (unsigned int i = 0; i < neighbors.size(); ++i) {
		//if (neighbors[i].pointer() == parent().pointer()) continue;

		avgHeading += neighbors[i]->heading();
		++neighborCount;
	}

	if (neighborCount == 0) return Math::Vector(0,0);

	avgHeading /= neighborCount;
	return (avgHeading - parent()->heading()).normal() * 3;
}