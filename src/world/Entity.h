#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>
#include <set>
#include <QPainter>
#include "world/View.h"
#include "math/Point.h"

//forward declaration
class World;

class Entity
{
public:
	typedef std::set<Entity*> ChildList;
	
protected:
	//subclass must redefine constructor, and call subclass()
	Entity(Entity* parent, std::string name="Entity");
	void subclass(std::string type);
	
	virtual ~Entity();

public:
	bool inherits(std::string type) const;

	//replace with region? or abstract intersects function?
	void setPos(const Math::Point& loc) { mLoc = loc; }
	Math::Point pos() const { return mLoc; }
	
	void remove() { mRemove = true; }
	
	Entity* parent() const { return mParent; }
	World* world() const { return mWorld; }
	
	bool visible() const { return mVisible; }

protected:
	void update(double secsElapsed);
	void paint(QPainter* p);
	
	void setVisible(bool v) { mVisible = v; }
	void setView(View* view) { mView = view; }
	View* view() const { return mView; }
	
private:
	void addChild(Entity* child) { mChildren.insert(child); }
	void delChild(Entity* child) { mChildren.erase(child); }
	
	virtual void updateEvent(double /*secsElapsed*/) {}
	
	std::vector<std::string> mSubclasses;
	
	Entity *mParent;
	World *mWorld;
	ChildList mChildren;
	
	std::string mName;
	Math::Point mLoc;
	
	View* mView;
	bool mVisible;
	
	bool mRemove;
};

inline Entity::Entity(Entity* parent, std::string name)
	:	mParent(parent),
		mName(name),
		mView(0),
		mVisible(false),
		mRemove(false)
{
	if (parent) {
		parent->addChild(this);
		mWorld = parent->world();
	}
}

#endif /*ENTITY_H_*/
