#ifndef BUILDING_H_
#define BUILDING_H_

#include <QVector>
#include "Entity.h"
#include "math/Point.h"
#include "EntityFactory.h"

class Wall;
class BuildingFactory;

class Building : public Entity
{
	ENTITY(Building)
	FACTORY(BuildingFactory)
public:
	Building(Pointer<Entity> parent, QString name="Building");
	virtual ~Building();

	void createWalls(const Math::Point* begin, const Math::Point* end);
	void addWall(Pointer<Wall> wall) { mWalls.push_back(wall); }
	const QVector<Pointer<Wall> > walls() const { return mWalls; }

private:
	QVector<Pointer<Wall> > mWalls;
};


class BuildingFactory : public SimpleEntityFactory<Building>
{
public:
	typedef EntityFactory::Properties Properties;

	void addChild(Pointer<Building> e, Pointer<Entity> child) const;

protected:
	Pointer<Entity> construct(Properties properties, Pointer<Entity> parent) const;
};

#endif /*BUILDING_H_*/
