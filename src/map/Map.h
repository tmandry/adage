#ifndef MAP_H_
#define MAP_H_

#include <string>
#include <map>
#include "Pointer.h"
#include "World.h"

class QFile;

class Map
{
public:
	Map(Pointer<World> parent);
	virtual ~Map();

	void load(QFile* file);
	void save(QFile* file);

private:
	Pointer<World> mWorld;
};

#endif /* MAP_H_ */
