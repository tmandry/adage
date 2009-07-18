#include <vector>
#include "Building.h"
#include "world/Wall.h"

Building::Building(Pointer<Entity> parent, QString name)
	:	Entity(parent, name)
{
	subclass();
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
		mWalls.push_back( (new Wall(pointer(), begin[0], begin[1]))->pointer() );
		return;
	}

	for (const Math::Point* i = begin; i < end-1; ++i) {
		Math::Point a, b;

		a = *i;
		b = *(i+1);

		mWalls.push_back( (new Wall(pointer(), a, b))->pointer() );
	}
}


Pointer<Entity> BuildingFactory::construct(Properties properties, Pointer<Entity> parent) const
{
	return (new Building(parent))->pointer();
}

void BuildingFactory::addChild(Pointer<Building> e, Pointer<Entity> child) const
{
	if (child->inherits<Wall>()) {
		e->addWall((Pointer<Wall>)child);
	}
	//TODO: error
}
