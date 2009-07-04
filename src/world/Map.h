/*
 * Map.h
 *
 *  Created on: Jun 21, 2009
 *      Author: tyler
 */

#ifndef MAP_H_
#define MAP_H_

#include <string>
#include "Pointer.h"
#include "World.h"

class QFile;
class QTextStream;

class Map {
public:
	Map(Pointer<World> parent, std::string filename = "");
	virtual ~Map();

	void open(std::string filename);
	void load();

private:
	void readNavMesh();
	void readBuildings();

	Pointer<World> mParent;
	QFile* mFile;
};

#endif /* MAP_H_ */
