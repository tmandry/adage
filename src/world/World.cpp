#include <algorithm>
#include <utility>
#include <cassert>
#include "World.h"
#include "Game.h"
#include "world/Wall.h"

World::World(Game* game, std::string name)
	:	Entity(Pointer<Entity>(), name),
		mGame(game),
		mCellSpace(this)
{
}

World::~World()
{
}

void World::removeEntity(std::string type, Pointer<Entity> e)
{
	EntityVector& list(mEntities[type]);
	assert(!list.empty());

	mCellSpace.remove(e, e->pos());

	//faster way to remove an element since this is an unordered list
	std::swap(
		*( std::find(list.begin(), list.end(), e) ),
		list.back()
	);
	list.pop_back();
}

void World::setBounds(double left, double top, double right, double bottom)
{
	mLeftBound = left;
	mTopBound = top;
	mRightBound = right;
	mBottomBound = bottom;

	if (!mBoundaries.empty()) for (int i = 0; i < 4; ++i) delete mBoundaries[i];
	mBoundaries.clear();

	//create invisible walls
	mBoundaries.push_back( new Wall(pointer(), left,top, right,top, false) );
	mBoundaries.push_back( new Wall(pointer(), left,top, left,bottom, false) );
	mBoundaries.push_back( new Wall(pointer(), left,bottom, right,bottom, false) );
	mBoundaries.push_back( new Wall(pointer(), right,bottom, right,top, false) );

	mCellSpace.partition();
}
