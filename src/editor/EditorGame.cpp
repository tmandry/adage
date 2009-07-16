#include <QTimer>
#include "EditorGame.h"
#include "world/World.h"

const int EditorGame::fps = 50;

EditorGame::EditorGame(CommWindow* comm)
	:	mComm(comm)
{
	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateWorld()));

	mTimer->setInterval(1000 / fps);
}

void EditorGame::log(QString msg)
{
	if (mComm) mComm->log(msg);
}

void EditorGame::setUpdating(bool on)
{
	if (on) {
		mTimer->start();
		mElapsed.start();
	} else
		mTimer->stop();

	mUpdating = on;
}

void EditorGame::start()
{
	setUpdating(true);
}

void EditorGame::stop()
{
	setUpdating(false);
}

void EditorGame::updateWorld()
{
	const double interval = 1.0 / fps;
	world()->update(mElapsed.restart() / 1000.0);

	emit worldUpdated();
}
