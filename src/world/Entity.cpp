#include "Entity.h"
#include "World.h"

Entity::~Entity()
{
	for (unsigned int i=0; i<mSubclasses.size(); ++i)
		world()->removeEntity(mSubclasses[i], this);
	
	for (ChildList::iterator i=mChildren.begin(); i!=mChildren.end(); ++i)
		delete (*i);
	
	if (parent()) parent()->delChild(this);
	if (mView) delete mView;
}

void Entity::subclass(std::string type)
{
	mSubclasses.push_back(type);
	world()->addEntity(type, this);
}

bool Entity::inherits(std::string type) const
{
	for (unsigned int i=0; i<mSubclasses.size(); ++i)
		if (mSubclasses[i] == type) return true;
	
	return false;
}

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
