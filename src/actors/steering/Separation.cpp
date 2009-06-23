/*
 * Separation.cpp
 *
 *  Created on: Jun 23, 2009
 *      Author: tyler
 */

#include "Separation.h"
#include "Pointer.h"

Separation::Separation(Pointer<Actor> parent, std::string targetType)
	:	SteeringBehavior(parent, 1.0),
		mTargetType(targetType)
{}

Math::Vector Separation::calculate()
{
	ConstEntityList<Entity> neighbors = parent()->world()->findEntities<Entity>(parent()->pos(), 10, mTargetType);

	Math::Vector force;
	for (unsigned int i = 0; i < neighbors.size(); ++i) {
		if (neighbors[i].pointer() != parent().pointer()) {
			Math::Vector toNeighbor = parent()->pos() - neighbors[i]->pos();

			//scale the force inversely proportional to neighbor's distance
			force += (toNeighbor.normal()/toNeighbor.length()) * 5;
		}
	}

	return force;
}
