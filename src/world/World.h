#ifndef WORLD_H_
#define WORLD_H_

#include <map>
#include <string>
#include <vector>
#include "Entity.h"

//forward declaration
class Game;

class World : public Entity
{
public:
	typedef std::vector<Entity*> EntityList;
	typedef std::map<std::string, EntityList> EntityMap;

	World(Game* game, std::string name="World");
	virtual ~World();
	
	void update(double secsElapsed) { Entity::update(secsElapsed); }
	void paint(QPainter* p) { Entity::paint(p); }
	
	void addEntity(std::string type, Entity* e) { mEntities[type].push_back(e); }
	void removeEntity(std::string type, Entity* e);
	
	const std::vector<Entity*>& findEntities(std::string type) { return mEntities[type]; }
	
	Game* game() const { return mGame; }

private:
	World* theWorld() { return this; }

	EntityMap mEntities;

	Game* mGame;
};

#endif /*WORLD_H_*/
