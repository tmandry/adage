#include "Entity.h"

#ifndef WORLD_H_
#define WORLD_H_

#include <cassert>
#include <utility>
#include <QMap>
#include <QString>
#include <QVector>
#include "EntityList.h"
#include "Pointer.h"
#include "world/GameBase.h"
#include "world/CellSpacePartition.h"
#include "math/const.h"
#include "nav/NavPath.h"

//forward declaration
class Wall;
class Map;
class NavSystem;

class World : public Entity
{
	ENTITY(World)
public:
	typedef QVector<Pointer<Entity> > EntityVector;
	typedef QHash<QString, EntityVector> EntityMap;

	World(GameBase* game, QString name="World");
	virtual ~World();

	void update(double secsElapsed) { Entity::update(secsElapsed); }
	void paint(QPainter* p) { Entity::paint(p); }

	void addEntity(QString type, Pointer<Entity> e) { assert(e != 0); mEntities[type].push_back(e); }
	void removeEntity(QString type, Pointer<Entity> e);
	//returns all immediate children
	Entity::ChildList _childEntities() const;

	template<class E>
	EntityList<E> findEntities() const
	{
		QString type = Entity::_className<E>();
		//const EntityVector* result(&mEntities[type]);
		//for (unsigned int i = 0; i < result.size(); ++i) assert(result[i] /*Not returning a deleted entity*/);
		return EntityList<E>(mEntities[type]);
	}

	//Note: Result invalidated upon next search
	template<class E>
	EntityList<E> findEntities(Math::Point p, double radius)
	{
		mCellSpace.findNeighbors(p, radius, Entity::_className<E>());
		//for (unsigned int i = 0; i < mCellSpace.result().size(); ++i) assert(mCellSpace.result()[i] /*Not returning a deleted entity*/);
		return EntityList<E>(mCellSpace.result());
	}

	template<class E>
	Pointer<E> findNearestEntity(Math::Point p, double maxDistance = Math::maxDouble)
	{
		return (Pointer<E>)mCellSpace.findNearest(p, Entity::_className<E>(), maxDistance);
	}

	//Should not be used by normal classes and functions
	template<class E>
	EntityList<E> _findEntities(Math::Point p, double radius, QString className)
	{
		mCellSpace.findNeighbors(p, radius, className);
		return EntityList<E>(mCellSpace.result());
	}

	bool findPath(NavPath& path, Math::Point start, Math::Point dest);

	GameBase* game() const { return mGame; }

	void setBounds(double left, double top, double right, double bottom);
	double leftBound() const { return mLeftBound; }
	double topBound() const { return mTopBound; }
	double rightBound() const { return mRightBound; }
	double bottomBound() const { return mBottomBound; }

	void updatePos(Pointer<Entity> e, Math::Point oldPos, Math::Point newPos) { mCellSpace.updatePos(e, oldPos, newPos); }

	void setNavSystem(NavSystem* nav);
	NavSystem* navSystem() { return mNav; }

private:
	friend class MapHandler;

	friend class CellSpacePartition;
	static bool inherits(Pointer<Entity> e, QString type) { return e->_inherits(type); }

	virtual Pointer<World> theWorld() { return Pointer<World>(this); }

	EntityMap mEntities;
	GameBase* mGame;
	QVector<Pointer<Wall> > mBoundaries;
	double mLeftBound, mTopBound, mRightBound, mBottomBound;

	CellSpacePartition mCellSpace;
	NavSystem* mNav;
};

inline void Entity::setPos(const Math::Point loc)
{
	world()->updatePos(pointer(), mLoc, loc);
	mLoc = loc;
}

#endif /*WORLD_H_*/
