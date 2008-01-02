#include <vector>
#include "Building.h"
#include "world/Wall.h"

Building::Building(Entity* parent, std::string name)
	:	Entity(parent, name)
{
	subclass("Building");
	
	mWalls.push_back( new Wall(this, -20,40, 20,40) );
	mWalls.push_back( new Wall(this, 20,40, 20,-40) );
	mWalls.push_back( new Wall(this, 20,-40, -20,-40) );
	mWalls.push_back( new Wall(this, -20,-40, -20,40) );
	
	setVisible(true);
}

Building::~Building()
{
}
