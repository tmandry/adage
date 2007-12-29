#include "Entity.h"

void Entity::update(double secsElapsed)
{
	this->updateEvent(secsElapsed);
	
	//update children
	std::vector<Entity*> deaths;
	for (ChildList::iterator i = mChildren.begin(); i != mChildren.end(); ++i) {
		(*i)->update(secsElapsed);
		if ((*i)->mRemove) deaths.push_back(*i);
	}
	
	//remove those scheduled for removal
	for (unsigned int i=0; i<deaths.size(); ++i)
		delete deaths[i];
}

void Entity::paint(QPainter* p)
{
	if (view()) view()->paint(p);
	
	//update children
	for (ChildList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
		if ((*i)->visible())
			(*i)->paint(p);
}
