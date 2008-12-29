#ifndef GAME_H_
#define GAME_H_

#include <QObject>
#include <QTime>
#include "world/World.h"

class QTimer;

class Game : public QObject
{
	Q_OBJECT

public:
	Game();
	virtual ~Game() { delete mWorld; }

	Pointer<World> world() const { return mWorld->world(); }

private slots:
	void updateWorld();

signals:
	void worldUpdated();

private:
	static const int fps = 50;

	World* mWorld;

	//fires events every update
	QTimer* mTimer;
	//measures actual time elapsed between updates
	QTime mElapsed;
};


#endif /*GAME_H_*/
