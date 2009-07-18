#ifndef GHOSTBUSTERSHQ_H_
#define GHOSTBUSTERSHQ_H_

#include <map>
#include "Entity.h"
#include "actors/Ghost.h"

class GhostBuster;

class GhostBustersHQ : public Entity {
	ENTITY(GhostBustersHQ)
	MINIMAL_FACTORY
public:
	GhostBustersHQ(Pointer<Entity> parent);
	virtual ~GhostBustersHQ() {}

	void newAgent(Pointer<GhostBuster> agent);
	void reassign(Pointer<GhostBuster> agent);
	void targetCaught(Pointer<GhostBuster> agent, Pointer<Ghost> target);

private:
	QString formatComm(QString msg) const;

	typedef std::map<Pointer<Ghost>, int> GhostAssnMap;
	GhostAssnMap mGhostAssignments;

	int mMaxAssignments; //max # of agents that can be assigned to a Ghost
	int mNumAgents;
	std::string mNextName;
};

#endif /* GHOSTBUSTERSHQ_H_ */
