#include <vector>
#include "Building.h"
#include "world/Wall.h"

Building::Building(Pointer<Entity> parent, std::string name)
	:	Entity(parent, name)
{
	subclass("Building");

	//mWalls.push_back( new Wall(pointer(), -100,100, 100,100) );
	mWalls.push_back( new Wall(pointer(), 125,-90, -125,-90) ); //t
	mWalls.push_back( new Wall(pointer(), -125,-90, -75,90) ); //l
	mWalls.push_back( new Wall(pointer(), 75,90, 125,-90) ); //r
	mWalls.push_back( new Wall(pointer(), -75,90, -25,140) ); //bl
	mWalls.push_back( new Wall(pointer(), 75,90, 25,140) ); //br

	mWalls.push_back( new Wall(pointer(), -25,140, -75,190) );
	mWalls.push_back( new Wall(pointer(), 25,140, 75,190) );
	mWalls.push_back( new Wall(pointer(), -75,190, -125,370) );
	mWalls.push_back( new Wall(pointer(), 75,190, 125,370) );
	mWalls.push_back( new Wall(pointer(), -125,370, 125,370) );


	//outer wall
	int oSz = 400;
	mWalls.push_back( new Wall(pointer(), -oSz,-oSz, oSz,-oSz));
	mWalls.push_back( new Wall(pointer(), oSz,-oSz, oSz,oSz));
	mWalls.push_back( new Wall(pointer(), oSz,oSz, -oSz,oSz));
	mWalls.push_back( new Wall(pointer(),-oSz,oSz, -oSz,-oSz));


	setVisible(true);
}

Building::~Building()
{
}
