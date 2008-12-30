#ifndef BUILDING_H_
#define BUILDING_H_

#include <vector>
#include "Entity.h"

class Wall;

class Building : public Entity
{
	ENTITY(Building)
public:
	Building(Pointer<Entity> parent, std::string name="Building");
	virtual ~Building();

private:
	std::vector<Wall*> mWalls;
};

#endif /*BUILDING_H_*/
