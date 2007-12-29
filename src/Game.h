#ifndef GAME_H_
#define GAME_H_

#include "world/Map.h"

class Game
{
private:
	Map mMap;
	
public:
	Game() {}
	virtual ~Game() {}
	
	Map& map() { return mMap; }
	const Map& map() const { return mMap; }
};


#endif /*GAME_H_*/
