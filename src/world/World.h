#ifndef WORLD_H_
#define WORLD_H_

#include <map>
#include <string>
#include <list>
#include "Entity.h"

//forward declaration
class Game;

class World : public Entity
{
public:
	typedef std::map<std::string, std::list<Entity*> > EntityMap;

	World(Game* game, std::string name="World");
	virtual ~World();
	
	void paint(QPainter* p) { Entity::paint(p); }
	
	void addEntity(std::string type, Entity* e) { mEntities[type].push_back(e); }
	void removeEntity(std::string type, Entity* e) { mEntities[type].remove(e); }
	
	const std::list<Entity*>& findEntities(std::string type) { return mEntities[type]; }
	
	Game* game() const { return mGame; }

private:
	EntityMap mEntities;

	Game* mGame;
};

#endif /*WORLD_H_*/
