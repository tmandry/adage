#include "World.h"

World::World(Game* game, std::string name)
	:	Entity(0, name),
		mGame(game)
{
}

World::~World()
{
}
