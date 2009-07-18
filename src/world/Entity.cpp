#include "Entity.h"
#include "World.h"

Entity::~Entity()
{
	assert(removed());

	for (ChildList::iterator i=mChildren.begin(); i!=mChildren.end(); ++i)
		(*i).free();

	if (parent()) parent()->delChild(mThis);
	if (mView) delete mView;
}

void Entity::subclass()
{
	QString type = this->_className();
	mSubclasses.insert(type);
	world()->addEntity(type, mThis);
}

bool Entity::_inherits(QString type) const
{
	return mSubclasses.contains(type);
}

void Entity::remove()
{
	//if already scheduled for removal, do nothing
	if (removed()) return;

	mRemove = true;

	//remove children
	for (ChildList::iterator i=mChildren.begin(); i!=mChildren.end(); ++i)
		(*i)->remove();

	//wipe away every trace..
	foreach (QString className, mSubclasses)
		world()->removeEntity(className, mThis);
}

void Entity::update(double secsElapsed)
{
	if (!removed()) this->updateEvent(secsElapsed);

	//update children
	std::vector<Pointer<Entity> > deaths;
	for (ChildList::iterator i = mChildren.begin(); i != mChildren.end(); ++i) {
		(*i)->update(secsElapsed);
		if ((*i)->mRemove) deaths.push_back(*i);
	}

	//remove those scheduled for removal
	for (unsigned int i=0; i<deaths.size(); ++i)
		deaths[i].free();
}

void Entity::paint(QPainter* p)
{
	if (view()) view()->paint(p);

	//update children
	for (ChildList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
		if ((*i)->visible())
			(*i)->paint(p);
}

QString Entity::formatComm(QString msg) const
{
	return /*"[" + QString(mName.c_str()) + "] " + */msg;
}
