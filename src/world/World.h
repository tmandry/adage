#ifndef WORLD_H_
#define WORLD_H_

#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <utility>
#include "Entity.h"
#include "detail/StaticCastIterator.h"

template<class E>
class EntityList
{
public:
	typedef StaticCastIterator<E*, typename std::vector<Entity*>::iterator > iterator;
	typedef StaticCastIterator<const E*, typename std::vector<Entity*>::const_iterator > const_iterator;
	typedef std::pair<iterator, iterator> type;
	typedef std::pair<const_iterator, const_iterator> const_type;
};


//forward declaration
class Game;

class World : public Entity
{
public:
	typedef std::vector<Entity*> EntityVector;
	typedef std::map<std::string, EntityVector> EntityMap;

	World(Game* game, std::string name="World");
	virtual ~World();
	
	void update(double secsElapsed) { Entity::update(secsElapsed); }
	void paint(QPainter* p) { Entity::paint(p); }
	
	void addEntity(std::string type, Entity* e) { mEntities[type].push_back(e); }
	void removeEntity(std::string type, Entity* e);
	
	template<class E>
	typename EntityList<E>::const_type findEntities(std::string type) const
	{
		EntityMap::const_iterator result = mEntities.find(type);
		assert(result != mEntities.end());
		return std::make_pair<>(result->second.begin(), result->second.end());
	}
	
	Game* game() const { return mGame; }

private:
	World* theWorld() { return this; }

	EntityMap mEntities;

	Game* mGame;
};
#endif /*WORLD_H_*/
