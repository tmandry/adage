/*
 * GhostBustersHQ.cpp
 *
 *  Created on: Jun 23, 2009
 *      Author: tyler
 */

#include <vector>
#include "GhostBustersHQ.h"
#include "actors/GhostBuster.h"
#include "math/rand.h"

GhostBustersHQ::GhostBustersHQ(Pointer<Entity> parent)
	:	Entity(parent)
{
	subclass("GhostBustersHQ");

	mMaxAssignments = 2;
}

//Reassigns a GhostBuster agent to a new target
void GhostBustersHQ::reassign(Pointer<GhostBuster> agent)
{
	//Note: The GhostBusters HQ is kept up to date with the latest Ghost tracking information
	//First update the max assignments to something reasonable based on the GB:G ratio
	int numGBs = world()->findEntities<Entity>("GhostBuster").size();
	int numGs = world()->findEntities<Entity>("Ghost").size();
	if (numGs > 0) mMaxAssignments = numGBs / numGs + 1;

	Pointer<Ghost> target;

	//No longer assigned to older target...
	target = agent->target();
	if (target) {
		GhostAssnMap::iterator result = mGhostAssignments.find(target);
		if (result != mGhostAssignments.end()) --(result->second);
	}


	//First try getting the nearest Ghost
	target = world()->findNearestEntity<Ghost>(agent->pos(), "Ghost", 50);
	if (target && mGhostAssignments[target] >= mMaxAssignments) target.release();

	//Otherwise, pick one around the agent..
	if (!target) {
		const ConstEntityList<Ghost> result = world()->findEntities<Ghost>(agent->pos(), 65, "Ghost");

		std::vector<Pointer<Ghost> > possibles;
		for (ConstEntityList<Ghost>::const_iterator i = result.begin(); i < result.end(); ++i)
			if (mGhostAssignments[(*i)] < mMaxAssignments) possibles.push_back(*i);

		if (!possibles.empty()) {
			int idx = Math::randInt(0, possibles.size()-1);
			target = possibles[idx];
		}
	}

	//Finally just pick a random ghost
	if (!target) {
		const ConstEntityList<Ghost> result = world()->findEntities<Ghost>("Ghost");

		std::vector<Pointer<Ghost> > possibles;
		for (ConstEntityList<Ghost>::const_iterator i = result.begin(); i < result.end(); ++i)
			if (mGhostAssignments[(*i)] < mMaxAssignments) possibles.push_back(*i);

		if (!possibles.empty()) {
			int idx = Math::randInt(0, possibles.size()-1);
			target = possibles[idx];
		}
	}

	//Should have picked a Ghost by now if one is available, now assign it and record the assignment
	agent->setTarget(target);
	if (target) ++mGhostAssignments[target];
}

void GhostBustersHQ::targetCaught(Pointer<Ghost> target)
{
	//remove from map
	mGhostAssignments.erase(target);
}
