#include <vector>
#include "Building.h"
#include "world/Wall.h"

Building::Building(Pointer<Entity> parent, std::string name)
	:	Entity(parent, name)
{
	subclass("Building");
	setVisible(true);
}

Building::~Building()
{
}

void Building::createWalls(const Math::Point* begin, const Math::Point* end)
{
	assert((end-begin) > 1);

	//just because
	if ((end-begin) == 2) {
		mWalls.push_back( new Wall(pointer(), begin[0], begin[1]));
		return;
	}

	for (const Math::Point* i = begin; i < end-1; ++i) {
		Math::Point a, b;

		a = *i;
		b = *(i+1);

		mWalls.push_back( new Wall(pointer(), a, b) );
	}
}
