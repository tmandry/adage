#ifndef WORLD_H_
#define WORLD_H_

#include "Entity.h"

//forward declaration
class Game;

class World : public Entity
{
public:
	World(Game* game, std::string name="World");
	virtual ~World();
	
	void paint(QPainter* p) { Entity::paint(p); }
	
	Game* game() const { return mGame; }

private:
	Game* mGame;
};

#endif /*WORLD_H_*/
