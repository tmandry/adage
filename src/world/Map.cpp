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
#include "nav/NavSystem.h"

Map::Map(Pointer<World> parent, std::string filename)
	:	mParent(parent),
		mFile(0)
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
	//read to first non-comment/blank line
	do { line = mFile->readLine().trimmed(); } while (line[0] == '#' || line.isEmpty());

	//read in world boundaries first
	double lb, tb, rb, bb;
	QTextStream(&line) >> lb >> tb >> rb >> bb;
	mParent->setBounds(lb, tb, rb, bb);

	readNavMesh();
	readBuildings();

	mFile->close();
}

void Map::readNavMesh()
{
	NavSystem* nav = new NavSystem;

	QString line;
	//read to first non-blank line
	do { line = mFile->readLine().trimmed(); } while (line.isEmpty());

	//read in nav nodes (polygons) until the first blank
	Math::Point* points = new Math::Point[100];
	while (!line.isEmpty()) {
		if (line[0] != '#') {
			//Every node has an id which is used later in the file, these must start at 1 and go up by 1 in order.
			int id;
			double x, y;
			QTextStream stream(&line);

			//Read in points
			stream >> id;
			int i = 0;
			while (!stream.atEnd()) {
				stream >> x;
				assert(!stream.atEnd());
				stream >> y;

				points[i++] = Math::Point(x,y);
				assert(i < 100);
			}

			//add it and check the id
			int actualId = nav->addNode(points, points+i);
			assert(id == actualId); //TODO better way of handling
		}
		line = mFile->readLine().trimmed();
	}

	delete[] points;


	//Next part is links - first the linking node id and then the ids of all the nodes it's linked to,
	//in order according to how the edges were ordered in the previous section.
	//Use 0 if there is no link.

	//read to first non-blank line
	do { line = mFile->readLine().trimmed(); } while (line.isEmpty());

	while (!line.isEmpty()) {
		if (line[0] != '#') {
			QTextStream stream(&line);
			int sourceId, destId;
			int link = 1;
			stream >> sourceId;

			while (!stream.atEnd()) {
				stream >> destId;
				if (destId != 0) nav->setLink(sourceId, link, destId);
				++link;
			}
		}
		line = mFile->readLine().trimmed();
	}

	//NavMesh is complete, assign it to the World
	mParent->setNavSystem(nav);
}

void Map::readBuildings()
{
	QString line;
	std::vector<Math::Point> points;
	Building* building = new Building(mParent);

	int blanks = 0;
	bool usedBuilding = false;

	while(true) {
		line = mFile->readLine().trimmed();
		if (line[0] == '#') continue;

		QTextStream stream(&line);

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
