#include "Ghost.h"
#include "Person.h"
#include "world/World.h"
#include "math/rand.h"

Ghost::Ghost(Math::Point pos, Entity* parent, std::string name)
	:	Actor(parent, name),
		mPursue(this),
		mWander(this)
{
	subclass("Ghost");
	
	setPos(pos);
	setMaxSpeed(9.0);
	
	addSteeringBehavior(&mPursue);
	addSteeringBehavior(&mWander);
	
	PersonView* view = new PersonView(this);
	view->setColor(Qt::green); // HULK SMASH!  -- 20070108 iank
	setView(view);
	setVisible(true);
	
	//pick a random person to pursue
	EntityList<Person>::const_type people = world()->findEntities<Person>("Person");
	int idx = Math::randInt(0, people.second-people.first);
	mPursue.setTarget(*(people.first + idx));
}
