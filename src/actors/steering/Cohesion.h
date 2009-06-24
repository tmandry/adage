/*
 * Cohesion.h
 *
 *  Created on: Jun 23, 2009
 *      Author: tyler
 */

#ifndef COHESION_H_
#define COHESION_H_

#include <string>
#include "SteeringBehavior.h"

class Cohesion: public SteeringBehavior {
public:
	Cohesion(Pointer<Actor> parent);
	virtual ~Cohesion();

	Math::Vector calculate();
};

#endif /* COHESION_H_ */
