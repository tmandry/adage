#include "Ghost.h"
#include "Person.h"

Ghost::Ghost(Math::Point pos, Entity* parent, std::string name)
	:	Actor(parent, name),
		mWander(this, 5.0)
{
	subclass("Ghost");
	
	setPos(pos);
	
	PersonView* view = new PersonView(this);
	view->setColor(Qt::red);
	setView(view);
	setVisible(true);
}
