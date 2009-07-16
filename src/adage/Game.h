#ifndef GAME_H_
#define GAME_H_

#include <QObject>
#include <QTime>
#include <QPoint>
#include "ui/CommWindow.h"
#include "world/World.h"
#include "world/GameBase.h"

class QTimer;
class Navigator;

class Game : public QObject, public GameBase
{
	Q_OBJECT

public:
	Game(CommWindow* comm);

	Pointer<CommWindow> comm() const { return mComm; }
	void log(QString msg) { mComm->log(msg); }

	void setNavvy(Navigator* n) { mNavvy = n; }

public slots:
	void navigate(QPointF point);

private slots:
	void updateWorld();

signals:
	void worldUpdated();

private:
	static const int fps = 50;

	Pointer<CommWindow> mComm;

	//fires events every update
	QTimer* mTimer;
	//measures actual time elapsed between updates
	QTime mElapsed;

	Navigator* mNavvy;
};

#endif /*GAME_H_*/
