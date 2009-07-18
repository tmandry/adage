/*
 * Cohesion.cpp
 *
 *  Created on: Jun 23, 2009
 *      Author: tyler
 */

#include "Cohesion.h"

Cohesion::Cohesion(Pointer<Actor> parent)
	:	SteeringBehavior(parent, 0.4, 60)
{
}

Cohesion::~Cohesion()
{
}

Math::Vector Cohesion::calculate()
{
	Math::Vector center; //calculate the center of the mass of actors
	int neighborCount = 0;

	EntityList<Actor> neighbors = parent()->neighbors();
	for (unsigned int i = 0; i < neighbors.size(); ++i) {
		if (!neighbors[i] || neighbors[i].pointer() == parent().pointer()) continue;

		center += Math::Vector(neighbors[i]->pos());
		++neighborCount;
	}
	center /= neighborCount;

	if (neighborCount == 0) return Math::Vector(0,0);

	//now seek to that position
	Math::Vector desiredVelocity = (Math::Point(center) - parent()->pos()).normal() * parent()->maxSpeed();
	return desiredVelocity - parent()->velocity();
}
