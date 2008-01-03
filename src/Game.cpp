#include <QTimer>
#include <QTime>
#include "Game.h"

Game::Game()
	:	mWorld(new World(this))
{
	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateWorld()));
	
	mTimer->setInterval(1000 / fps);
	mElapsed.start();
	mTimer->start();
}

void Game::updateWorld()
{
	double secsElapsed = mElapsed.restart();
	mWorld->update(secsElapsed);
	
	emit worldUpdated();
}
