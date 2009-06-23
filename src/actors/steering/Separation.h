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
	Separation(Pointer<Actor> parent, std::string targetType);
	virtual ~Separation() {}

	void setTargetType(std::string targetType) { mTargetType = targetType; }

	Math::Vector calculate();

private:
	std::string mTargetType;
};

#endif /* SEPARATION_H_ */
