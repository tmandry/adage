#ifndef BUILDING_H_
#define BUILDING_H_

#include <vector>
#include "Entity.h"
#include "math/Point.h"

class Wall;

class Building : public Entity
{
	ENTITY(Building)
public:
	Building(Pointer<Entity> parent, std::string name="Building");
	virtual ~Building();

	void createWalls(const std::vector<Math::Point>& points);

private:
	std::vector<Wall*> mWalls;
};

#endif /*BUILDING_H_*/
