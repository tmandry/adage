#include "GameBase.h"
#include "World.h"

GameBase::GameBase()
{
	World* w = new World(this);
	mWorld = w->world();
}

GameBase::~GameBase()
{
}

void GameBase::resetWorld()
{
	World* w = new World(this);
	mWorld = w->world();
}
