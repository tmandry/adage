#ifndef GAME_H_
#define GAME_H_

#include <QObject>
#include <QTime>
#include <QPoint>
#include "ui/CommWindow.h"
#include "world/World.h"

class QTimer;
class Navigator;

class Game : public QObject
{
	Q_OBJECT

public:
	Game(CommWindow* comm);
	virtual ~Game() { delete mWorld; }

	Pointer<World> world() const { return mWorld->world(); }
	Pointer<CommWindow> comm() const { return mComm; }

	void setNavvy(Navigator* n) { mNavvy = n; }

public slots:
	void navigate(QPointF point);

private slots:
	void updateWorld();

signals:
	void worldUpdated();

private:
	static const int fps = 50;

	World* mWorld;
	Pointer<CommWindow> mComm;

	//fires events every update
	QTimer* mTimer;
	//measures actual time elapsed between updates
	QTime mElapsed;

	Navigator* mNavvy;
};

inline void Entity::printComm(QString msg) const
{
	world()->game()->comm()->log(formatComm(msg));
}

#endif /*GAME_H_*/
