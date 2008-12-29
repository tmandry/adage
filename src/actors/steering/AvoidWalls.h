/*
 * AvoidWalls.h
 *
 *  Created on: Dec 27, 2008
 *      Author: tyler
 */

#ifndef AVOIDWALLS_H_
#define AVOIDWALLS_H_

#include "SteeringBehavior.h"

class AvoidWalls: public SteeringBehavior {
public:
	AvoidWalls(Pointer<Actor> parent);
	virtual ~AvoidWalls();

	Math::Vector calculate();
};

#endif /* AVOIDWALLS_H_ */
