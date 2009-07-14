#include <vector>
#include <string>
#include <iostream>
#include <QFile>
#include <QString>
#include <QtXml>
#include "Map.h"
#include "MapHandler.h"
#include "MapGenerator.h"
#include "world/Building.h"
#include "math/Point.h"
#include "nav/NavSystem.h"

Map::Map(Pointer<World> world)
	:	mWorld(world)
{
}

Map::~Map()
{
}

void Map::load(QFile* file)
{
	QXmlSimpleReader reader;
	MapHandler handler(mWorld);
	reader.setContentHandler(&handler);
	reader.setErrorHandler(&handler);

	reader.parse(QXmlInputSource(file));
}

void Map::save(QFile* file)
{
	MapGenerator generator(mWorld);
	generator.write(file);
}
