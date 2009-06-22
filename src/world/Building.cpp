#include <vector>
#include "Building.h"
#include "world/Wall.h"

Building::Building(Pointer<Entity> parent, std::string name)
	:	Entity(parent, name)
{
	subclass("Building");

	/*//mWalls.push_back( new Wall(pointer(), -100,100, 100,100) );
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
	mWalls.push_back( new Wall(pointer(),-oSz,oSz, -oSz,-oSz));*/


	setVisible(true);
}

Building::~Building()
{
}

void Building::createWalls(const std::vector<Math::Point>& points)
{
	assert(points.size() > 1);

	//just because
	if (points.size() == 2) {
		mWalls.push_back( new Wall(pointer(), points[0], points[1]));
		return;
	}

	for (int i = 0; i < points.size()-1; ++i) {
		Math::Point a, b;

		a = points[i];
		b = points[i+1];

		mWalls.push_back( new Wall(pointer(), a, b) );
	}
}
