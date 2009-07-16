#include "GhostTrap.h"
#include "actors/Ghost.h"
#include "world/GhostBustersHQ.h"

GhostTrap::GhostTrap(Pointer<Entity> parent, Math::Point pos, std::string name)
	:	LimitedLifetimeTrigger(parent, 20.0, name),
		mDetonated(false),
		mKillTimer(0),
		mKillCount(0)
{
	subclass("GhostTrap");

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
	if (target->inherits("Ghost")) {
		/*printComm("Ghost has been caught by a trap and sent back into the Underworld!");

		target->remove();
		this->remove(); //traps only fire once...*/

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
			ConstEntityList<Ghost> victims = world()->findEntities<Ghost>(pos(), mView->radius(), "Ghost");

			for (unsigned int i = 0; i < victims.size(); ++i) {
				victims[i]->remove();
				++mKillCount;
				ConstEntityList<GhostBustersHQ> hq = world()->findEntities<GhostBustersHQ>("GhostBustersHQ");
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
