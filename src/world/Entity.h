#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>
#include <set>
#include <QPainter>
#include <QObject>
#include <QString>
#include <cassert>
#include "world/View.h"
#include "math/Point.h"
#include "Pointer.h"
#include "ui/CommWindow.h"

#define ENTITY(type) \
	private: \
	Pointer<type> _mThis; \
	Pointer<type> pointer() const { assert(_mThis); return _mThis; } \
	virtual void subclass(std::string t) \
	{ \
		_mThis = Pointer<type>(this); \
		Entity::subclass( t ); \
	}

//forward declaration
class World;

class Entity
{
public:
	typedef std::set<Pointer<Entity> > ChildList;

protected:
	//subclasses MUST redefine constructor, and call subclass()
	Entity(Pointer<Entity> parent, std::string name="Entity");
	virtual void subclass(std::string type);

public:
	bool inherits(std::string type) const;
	void setName(std::string name) { mName = name; }
	std::string name() const { return mName; }

	//replace with region? or abstract intersects function?
	inline void setPos(const Math::Point loc); //definition in World.h
	Math::Point pos() const { return mLoc; }

	void remove();
	bool removed() { return mRemove; }

	Pointer<Entity> parent() const { return mParent; }
	Pointer<World> world() const { return mWorld; }
	/*template<class T>
	Pointer<T> pointer() { return Pointer<T>::staticPointerCast(mThis); }*/

	bool visible() const { return mVisible; }

protected:
	virtual ~Entity();
	Pointer<Entity> pointer() { return mThis; }

	friend void Pointer<Entity>::free() const;

	void update(double secsElapsed);
	void paint(QPainter* p);

	void setVisible(bool v) { mVisible = v; }
	void setView(View* view) { if (mView) delete mView; mView = view; }
	View* view() const { return mView; }

	inline void printComm(QString msg) const; //definition in World.h

private:
	virtual Pointer<World> theWorld() { return mWorld; }

	void addChild(Pointer<Entity> child) { assert(!removed()); mChildren.insert(child); }
	void delChild(Pointer<Entity> child) { mChildren.erase(child); }

	virtual void updateEvent(double /*secsElapsed*/) {}

	virtual QString formatComm(QString msg) const { return /*"[" + QString(mName.c_str()) + "] " + */msg; }

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
		mLoc(0,0),
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
