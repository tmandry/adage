#ifndef GAMEBASE_H_
#define GAMEBASE_H_

#include <QString>
#include "Pointer.h"

class World;

class GameBase
{
public:
	GameBase();
	virtual ~GameBase();

	Pointer<World> world() const { return mWorld; }
	virtual void log(QString msg) =0;

private:
	Pointer<World> mWorld;
};

#endif /* GAMEBASE_H_ */
