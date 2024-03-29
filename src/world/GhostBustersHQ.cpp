#include <vector>
#include <QString>
#include "GhostBustersHQ.h"
#include "world/GameBase.h"
#include "actors/GhostBuster.h"
#include "math/rand.h"

GhostBustersHQ::GhostBustersHQ(Pointer<Entity> parent)
	:	Entity(parent, "GhostBustersHQ"),
		mNumAgents(0),
		mNextName("A")
{
	subclass();

	mMaxAssignments = 2;

	printComm("GhostBusters HQ ready.");
}

//GhostBuster agents register themselves to receive their name
void GhostBustersHQ::newAgent(Pointer<GhostBuster> agent)
{
	++mNumAgents;
	printComm(QString("Agent <font color='yellow'>%1</font> registered. Now <font color='#eebb00'>%2</font> agents.").arg(mNextName.c_str()).arg(mNumAgents));
	agent->setName(mNextName.c_str());
	++mNextName[0];
}

//Reassigns a GhostBuster agent to a new target
void GhostBustersHQ::reassign(Pointer<GhostBuster> agent)
{
	//Note: The GhostBusters HQ is kept up to date with the latest Ghost tracking information
	//First update the max assignments to something reasonable based on the GB:G ratio
	int numGBs = world()->findEntities<GhostBuster>().size();
	int numGs = world()->findEntities<Ghost>().size();
	if (numGs > 0) mMaxAssignments = numGBs / numGs + 1;

	Pointer<Ghost> target;

	//No longer assigned to older target...
	target = agent->target();
	if (target) {
		GhostAssnMap::iterator result = mGhostAssignments.find(target);
		if (result != mGhostAssignments.end()) --(result->second);
	}


	//First try getting the nearest Ghost
	target = world()->findNearestEntity<Ghost>(agent->pos(), 50);
	if (target && mGhostAssignments[target] >= mMaxAssignments) target.release();

	//Otherwise, pick one around the agent..
	if (!target) {
		const EntityList<Ghost> result = world()->findEntities<Ghost>(agent->pos(), 65);

		std::vector<Pointer<Ghost> > possibles;
		for (EntityList<Ghost>::const_iterator i = result.begin(); i < result.end(); ++i)
			if (mGhostAssignments[(*i)] < mMaxAssignments) possibles.push_back(*i);

		if (!possibles.empty()) {
			int idx = Math::randInt(0, possibles.size()-1);
			target = possibles[idx];
		}
	}

	//Finally just pick a random ghost
	if (!target) {
		const EntityList<Ghost> result = world()->findEntities<Ghost>();

		std::vector<Pointer<Ghost> > possibles;
		for (EntityList<Ghost>::const_iterator i = result.begin(); i < result.end(); ++i)
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

void GhostBustersHQ::targetCaught(Pointer<GhostBuster> agent, Pointer<Ghost> target)
{
	//remove from map
	mGhostAssignments.erase(target);

	int numGs = world()->findEntities<Ghost>().size();
	if (agent) printComm(QString("Threat eliminated by Agent <font color='yellow'>%1</font>. <font color='lightgreen'>%2</font> remain.").arg(agent->name()).arg(numGs));
	if (numGs == 0) {
		int numCs = world()->findEntities<Person>().size();
		printComm(QString("Mission accomplished! <font color='#0066ff'>%1</font> civilians saved by the Ghost Busters.").arg(numCs));
	}

}

QString GhostBustersHQ::formatComm(QString msg) const
{
	return "[<font color='#ffcc00'>GBHQ</font>] " + msg;
}
