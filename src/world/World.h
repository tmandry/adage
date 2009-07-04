#ifndef WORLD_H_
#define WORLD_H_

#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <utility>
#include "Entity.h"
#include "EntityList.h"
#include "Pointer.h"
#include "world/CellSpacePartition.h"
#include "math/const.h"
#include "nav/NavPath.h"

//forward declaration
class Game;
class Wall;
class Map;
class NavSystem;

class World : public Entity
{
public:
	typedef std::vector<Pointer<Entity> > EntityVector;
	typedef std::map<std::string, EntityVector> EntityMap;

	World(Game* game, std::string name="World");
	virtual ~World();

	void update(double secsElapsed) { Entity::update(secsElapsed); }
	void paint(QPainter* p) { Entity::paint(p); }

	void addEntity(std::string type, Pointer<Entity> e) { assert(e != 0); mEntities[type].push_back(e); }
	void removeEntity(std::string type, Pointer<Entity> e);

	template<class E>
	ConstEntityList<E> findEntities(std::string type) const
	{
		EntityMap::const_iterator result = mEntities.find(type);
		assert(result != mEntities.end());
		for (unsigned int i = 0; i < result->second.size(); ++i) assert(result->second[i] /*Not returning a deleted entity*/);
		return ConstEntityList<E>(result->second);
	}

	//Note: Result invalidated upon next search
	template<class E>
	ConstEntityList<E> findEntities(Math::Point p, double radius, std::string type)
	{
		mCellSpace.findNeighbors(p, radius, type);
		for (unsigned int i = 0; i < mCellSpace.result().size(); ++i) assert(mCellSpace.result()[i] /*Not returning a deleted entity*/);
		return ConstEntityList<E>(mCellSpace.result());
	}

	template<class E>
	Pointer<E> findNearestEntity(Math::Point p, std::string type, double maxDistance = Math::maxDouble)
	{
		return (Pointer<E>)mCellSpace.findNearest(p, type, maxDistance);
	}

	bool findPath(NavPath& path, Math::Point start, Math::Point dest);

	Game* game() const { return mGame; }

	double leftBound() const { return mLeftBound; }
	double topBound() const { return mTopBound; }
	double rightBound() const { return mRightBound; }
	double bottomBound() const { return mBottomBound; }

	void updatePos(Pointer<Entity> e, Math::Point oldPos, Math::Point newPos) { mCellSpace.updatePos(e, oldPos, newPos); }

	void setNavSystem(NavSystem* nav);
	NavSystem* navSystem() { return mNav; }

private:
	friend class Map;
	void setBounds(double left, double top, double right, double bottom);

	virtual Pointer<World> theWorld() { return Pointer<World>(this); }

	EntityMap mEntities;
	Game* mGame;
	std::vector<Wall*> mBoundaries;
	double mLeftBound, mTopBound, mRightBound, mBottomBound;

	CellSpacePartition mCellSpace;
	NavSystem* mNav;
};

inline void Entity::setPos(const Math::Point loc)
{
	world()->updatePos(pointer(), mLoc, loc);
	mLoc = loc;
}

#include "Game.h"

#endif /*WORLD_H_*/
