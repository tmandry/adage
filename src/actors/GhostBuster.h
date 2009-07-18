#ifndef GHOSTBUSTER_H_
#define GHOSTBUSTER_H_

#include <string>
#include "Pointer.h"
#include "Actor.h"
#include "Person.h"
#include "Ghost.h"
#include "steering/Wander.h"
#include "steering/Pursue.h"

class GhostBustersHQ;

class GhostBuster : public Actor {
	ENTITY(GhostBuster)
	AUTO_FACTORY
public:
	GhostBuster(Math::Point pos, Pointer<Entity> parent, QString name = "GhostBuster");
	virtual ~GhostBuster();

	void setTarget(Pointer<Ghost> target);
	Pointer<Ghost> target() const { return mTarget; }

private:
	friend class PersonView;

	virtual void updateEvent(double secsElapsed);

	PersonView* mView;

	Pointer<GhostBustersHQ> mHQ;

	Wander* mWander;
	Pursue* mPursue;
	Pointer<Ghost> mTarget;

	int mKillCount;
};

/*class GBView : public PersonView {
public:
	GBView(Pointer<Actor> parent):
	virtual ~GBView() {}

	void paint(QPainter* p);
};*/

#endif /* GHOSTBUSTER_H_ */
