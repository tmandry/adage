#include <vector>
#include "Building.h"
#include "world/Wall.h"

Building::Building(Pointer<Entity> parent, QString name)
	:	Entity(parent, name)
{
	subclass();
	setVisible(true);
	setMovable(false);
}

Building::~Building()
{
}

void Building::createWalls(QList<Math::Point> points)
{
	assert((points.size()) > 1);

	if (points.size() == 2) {
		mWalls.push_back( (new Wall(pointer(), points[0], points[1]))->pointer() );
		return;
	}

	for (QList<Math::Point>::iterator i = points.begin(); i < points.end()-1; ++i) {
		Math::Point a, b;

		a = *i;
		b = *(i+1);

		mWalls.push_back( (new Wall(pointer(), a, b))->pointer() );
	}
}


QSet<QString> BuildingFactory::getPropertyNames() const
{
	static QSet<QString> names;
	if (names.isEmpty()) {
		names = SimpleEntityFactory<Building>::getPropertyNames();
		names.remove("movable");
		//pos?
	}

	return names;
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
