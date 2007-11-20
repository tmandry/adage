#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include "world/wall.h"

class Map
{
public:
	typedef std::vector<Wall> WallList;
	
private:
	//all the walls on the map
	//TODO: Maybe have a multimap with keys being zones and values being the walls
	WallList mWalls;
	
public:
	Map() {}
	virtual ~Map() {}
	
	void addWall(const Wall& wall)
	{
		mWalls.push_back(wall);
	}
	const WallList& walls() const { return mWalls; }
};

#endif /*MAP_H_*/
