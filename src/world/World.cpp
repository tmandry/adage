#include <algorithm>
#include <utility>
#include "World.h"

World::World(Game* game, std::string name)
	:	Entity(0, name),
		mGame(game)
{
}

World::~World()
{
}

void World::removeEntity(std::string type, Entity* e)
{
	EntityList& list(mEntities[type]);
	
	//faster way to remove an element since this is an unordered list
	std::swap(
		*( std::find(list.begin(), list.end(), e) ),
		*( list.end() )
	);
	list.pop_back();
}
