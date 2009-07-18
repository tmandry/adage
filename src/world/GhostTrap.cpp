#include "GhostTrap.h"
#include "actors/Ghost.h"
#include "world/GhostBustersHQ.h"

GhostTrap::GhostTrap(Math::Point pos, Pointer<Entity> parent, QString name)
	:	LimitedLifetimeTrigger(parent, 20.0, name),
		mDetonated(false),
		mKillTimer(0),
		mKillCount(0)
{
	subclass();

	setPos(pos);
	setTriggerRegion(new TriggerRegionCircle(pos, 7));
	mView = new CircleView(pointer(), 7, Qt::darkGreen, 0.5);
	setView(mView);
	setVisible(true);
}

GhostTrap::~GhostTrap() {
}

void GhostTrap::act(Pointer<Entity> target)
{
	if (target->inherits<Ghost>()) {
		//BOOM
		detonate();
	}
}

void GhostTrap::updateEvent(double secsElapsed)
{
	const double killRadius = 26;
	const double expandRate = 13; //units per second
	const double killInterval = 0.25; //how often to kill ghosts within our radius (to save CPU)

	if (mDetonated) {
		//first expand explosion radius
		mView->setRadius(mView->radius() + expandRate*secsElapsed);

		//if we've reached the end of the explosion, flag for removal and ensure we do one last killing!
		if (mView->radius() >= killRadius) {
			this->remove();
			mKillTimer = killInterval;
		}

		mKillTimer += secsElapsed;

		//perform executions if it's time
		if (mKillTimer >= killInterval) {
			EntityList<Ghost> victims = world()->findEntities<Ghost>(pos(), mView->radius());

			for (unsigned int i = 0; i < victims.size(); ++i) {
				victims[i]->remove();
				++mKillCount;
				EntityList<GhostBustersHQ> hq = world()->findEntities<GhostBustersHQ>();
				if (hq.size() != 0) hq[0]->targetCaught(Pointer<GhostBuster>(), victims[i]);
			}
		}

		if (removed()) printComm(QString("<font color='#00b000'>Ghost trap</font> <font color='#e50000'>DETONATED</font>, Killing <font color='lightgreen'>%1</font> ghosts!").arg(mKillCount));
	}

	LimitedLifetimeTrigger::updateEvent(secsElapsed);
}

void GhostTrap::detonate()
{
	mView = new CircleView(pointer(), mView->radius(), Qt::white, 1.0);
	setView(mView);
	mDetonated = true;
}
