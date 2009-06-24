/*
 * Separation.h
 *
 *  Created on: Jun 23, 2009
 *      Author: tyler
 */

#ifndef SEPARATION_H_
#define SEPARATION_H_

#include <string>
#include "SteeringBehavior.h"

class Separation: public SteeringBehavior {
public:
	Separation(Pointer<Actor> parent);
	virtual ~Separation() {}

	Math::Vector calculate();
};

#endif /* SEPARATION_H_ */
