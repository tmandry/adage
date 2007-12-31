#ifndef BUILDING_H_
#define BUILDING_H_

#include <vector>
#include "Entity.h"

class Wall;

class Building : public Entity
{
public:
	Building(Entity* parent, std::string name="Building");
	virtual ~Building();
	
private:
	std::vector<Wall*> mWalls;
};

#endif /*BUILDING_H_*/
