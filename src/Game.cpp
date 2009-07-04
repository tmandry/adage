#include <QTimer>
#include <QTime>
#include "Game.h"
#include "ui/Blueprint.h"
#include "actors/Navigator.h"

Game::Game(CommWindow* comm)
	:	mWorld(new World(this)),
		mComm(comm)
{
	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateWorld()));

	mTimer->setInterval(1000 / fps);
	mElapsed.start();
	mTimer->start();
}

void Game::updateWorld()
{
	double secsElapsed = mElapsed.restart() / 1000.0;
	mWorld->update(secsElapsed);

	emit worldUpdated();
	//Blueprint::bp->repaint();
}

void Game::navigate(QPointF point)
{
	if (mNavvy) mNavvy->goTo(Math::Point(point));
}
