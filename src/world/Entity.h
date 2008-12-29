#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>
#include <set>
#include <QPainter>
#include <QObject>
#include "world/View.h"
#include "math/Point.h"
#include "Pointer.h"

//forward declaration
class World;

class Entity
{
public:
	typedef std::set<Pointer<Entity> > ChildList;

protected:
	//subclass must redefine constructor, and call subclass()
	Entity(Pointer<Entity> parent, std::string name="Entity");
	void subclass(std::string type);

	virtual ~Entity();

public:
	bool inherits(std::string type) const;
	std::string name() const { return mName; }

	//replace with region? or abstract intersects function?
	void setPos(const Math::Point& loc) { mLoc = loc; }
	Math::Point pos() const { return mLoc; }

	void remove();
	bool removed() { return mRemove; }

	Pointer<Entity> parent() const { return mParent; }
	Pointer<World> world() const { return mWorld; }
	Pointer<Entity> pointer() { return mThis; }

	bool visible() const { return mVisible; }

protected:
	friend void Pointer<Entity>::free() const;

	void update(double secsElapsed);
	void paint(QPainter* p);

	void setVisible(bool v) { mVisible = v; }
	void setView(View* view) { mView = view; }
	View* view() const { return mView; }

private:
	virtual Pointer<World> theWorld() { return mWorld; }

	void addChild(Pointer<Entity> child) { mChildren.insert(child); }
	void delChild(Pointer<Entity> child) { mChildren.erase(child); }

	virtual void updateEvent(double /*secsElapsed*/) {}

	std::vector<std::string> mSubclasses;

	Pointer<Entity> mParent;
	Pointer<World> mWorld;
	ChildList mChildren;
	Pointer<Entity> mThis;

	std::string mName;
	Math::Point mLoc;

	View* mView;
	bool mVisible;

	bool mRemove;
};

#include "World.h"

inline Entity::Entity(Pointer<Entity> parent, std::string name)
	:	mParent(parent),
		mThis(this),
		mName(name),
		mView(0),
		mVisible(false),
		mRemove(false)
{
	if (parent) {
		parent->addChild(mThis);
		mWorld = parent->theWorld();
	} else {
		mWorld = Pointer<World>::staticPointerCast(Pointer<Entity>(this));
	}
}

#endif /*ENTITY_H_*/
