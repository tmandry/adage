/*
 * AvoidWalls.h
 *
 *  Created on: Dec 27, 2008
 *      Author: tyler
 */

#ifndef AVOIDWALLS_H_
#define AVOIDWALLS_H_

#include <vector>
#include "SteeringBehavior.h"
#include "math/Vector.h"

class AvoidWalls: public SteeringBehavior {
public:
	AvoidWalls(Pointer<Actor> parent);
	virtual ~AvoidWalls();

	Math::Vector calculate();

private:
	int mCycle;
	std::vector<Math::Vector> mResults;
};

#endif /* AVOIDWALLS_H_ */
