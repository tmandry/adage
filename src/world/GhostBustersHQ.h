/*
 * GhostBustersHQ.h
 *
 *  Created on: Jun 23, 2009
 *      Author: tyler
 */

#ifndef GHOSTBUSTERSHQ_H_
#define GHOSTBUSTERSHQ_H_

#include <map>
#include "Entity.h"
#include "actors/Ghost.h"

class GhostBuster;

class GhostBustersHQ : public Entity {
public:
	GhostBustersHQ(Pointer<Entity> parent);
	virtual ~GhostBustersHQ() {}

	void reassign(Pointer<GhostBuster> agent);
	void targetCaught(Pointer<Ghost> target);

	void updateEvent(double /*secsElapsed*/) {}

private:
	typedef std::map<Pointer<Ghost>, int> GhostAssnMap;
	GhostAssnMap mGhostAssignments;

	int mMaxAssignments; //max # of agents that can be assigned to a Ghost
};

#endif /* GHOSTBUSTERSHQ_H_ */
