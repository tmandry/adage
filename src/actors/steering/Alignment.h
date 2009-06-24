/*
 * Alignment.h
 *
 *  Created on: Jun 23, 2009
 *      Author: tyler
 */

#ifndef ALIGNMENT_H_
#define ALIGNMENT_H_

#include "SteeringBehavior.h"

class Alignment: public SteeringBehavior {
public:
	Alignment(Pointer<Actor> parent);
	virtual ~Alignment();

	Math::Vector calculate();
};

#endif /* ALIGNMENT_H_ */
