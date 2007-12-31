#ifndef GAME_H_
#define GAME_H_

#include "world/World.h"

class Game
{
public:
	Game(): mWorld(new World(this)) {}
	virtual ~Game() {}
	
	World* world() const { return mWorld; }

private:
	World* mWorld;
};


#endif /*GAME_H_*/
