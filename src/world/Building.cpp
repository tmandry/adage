#include <vector>
#include "Building.h"
#include "world/Wall.h"

Building::Building(Entity* parent, std::string name)
	:	Entity(parent, name)
{
	subclass("Building");

	//mWalls.push_back( new Wall(this, -100,100, 100,100) );
	mWalls.push_back( new Wall(this, 50,75, 100,-75) );
	mWalls.push_back( new Wall(this, 100,-75, -100,-75) );
	mWalls.push_back( new Wall(this, -100,-75, -50,75) );
	mWalls.push_back( new Wall(this, -50,75, -10, 125) );
	mWalls.push_back( new Wall(this, 50,75, 10, 125) );

	setVisible(true);
}

Building::~Building()
{
}
