/*
 * Map.cpp
 *
 *  Created on: Jun 21, 2009
 *      Author: tyler
 */

#include <vector>
#include <string>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "Map.h"
#include "world/Building.h"
#include "math/Point.h"

Map::Map(Pointer<World> parent, std::string filename)
	:	mParent(parent)
{
	if (!filename.empty()) open(filename);
}

Map::~Map() {
	if (mFile) delete mFile;
}

void Map::open(std::string filename)
{
	if (mFile) {
		delete mFile;
		mFile = 0;
	}

	mFile = new QFile( filename.c_str() );
	bool result = mFile->open(QIODevice::ReadOnly | QIODevice::Text);
	assert(result); //TODO error handling
}

void Map::load()
{
	QString line;
	std::vector<Math::Point> points;
	Building* building = new Building(mParent);

	int blanks = 0;
	bool usedBuilding = false;

	while(true) {
		line = mFile->readLine();
		if (line[0] == '#') continue;

		QTextStream stream(&line);
		stream.skipWhiteSpace();

		if (!stream.atEnd()) {
			blanks = 0;

			double x, y;
			stream >> x >> y;
			points.push_back(Math::Point(x,y));
		} else {
			++blanks;

			if (!points.empty()) {
				building->createWalls(points);
				points.clear();
				usedBuilding = true;
			}

			//now check for EOF
			if (mFile->atEnd()) break;

			if (blanks > 1 && usedBuilding) { //2 blanks means new building
				building = new Building(mParent);
				usedBuilding = false;
			}
		}
	}
}
