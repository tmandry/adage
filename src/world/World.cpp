#include <algorithm>
#include <utility>
#include <cassert>
#include "World.h"
#include "Game.h"

World::World(Game* game, std::string name)
	:	Entity(Pointer<Entity>(), name),
		mGame(game)
{
}

World::~World()
{
}

void World::removeEntity(std::string type, Pointer<Entity> e)
{
	EntityVector& list(mEntities[type]);
	assert(!list.empty());

	//faster way to remove an element since this is an unordered list
	std::swap(
		*( std::find(list.begin(), list.end(), e) ),
		list.back()
	);
	list.pop_back();
}
