#include "GhostPortal.h"
#include "actors/Ghost.h"

DormantGhostPortal::DormantGhostPortal(Pointer<Entity> parent, Math::Point pos)
	:	LimitedLifetimeTrigger(parent, 35.0)
{
	subclass("DormantGhostPortal");

	setPos(pos);
	setTriggerRegion(new TriggerRegionCircle(pos, 3.5));
	setView(new CircleView(pointer(), 3.5, Qt::gray, 0.3));
	setVisible(true);

	printComm("Portal created, lying dormant...");
}

void DormantGhostPortal::act(Pointer<Entity> target)
{
	if (target->inherits("Person")) {
		//first of all, this person just opened a portal to the underworld. that's not the kind of thing you're going to survive.
		target->remove();

		printComm("<font color='#ff0000'>Ghost portal</font> has been opened to the underworld! +<font color='lightgreen'>10</font> ghosts.");

		//Cosmic tear!
		new GhostPortal(world(), pos());
		this->remove();
	}
}

GhostPortal::GhostPortal(Pointer<Entity> parent, Math::Point pos)
	:	Entity(parent, "GhostPortal"),
		mTimer(0),
		mGhostsToSpawn(10)
{
	subclass("Portal");

	setPos(pos);
	setView(new CircleView(pointer(), 5.0, Qt::red, 1.5));
	setVisible(true);
}

//Spawn a new ghost after every interval
void GhostPortal::updateEvent(double secsElapsed)
{
	const double interval = 0.3;

	mTimer += secsElapsed;
	if (mTimer >= interval) {
		new Ghost(pos(), world());
		--mGhostsToSpawn;
		mTimer -= interval; //if we went past the interval this will keep track of that so the next ghost spawns in time
	}

	if (mGhostsToSpawn == 0) {
		this->remove();
	}
}

