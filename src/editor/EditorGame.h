#ifndef EDITORGAME_H_
#define EDITORGAME_H_

#include "world/GameBase.h"
#include <QObject>
#include <QTime>
#include "ui/CommWindow.h"

class QTimer;

class EditorGame: public QObject, public GameBase
{
	Q_OBJECT
public:
	EditorGame(CommWindow* comm);
	virtual ~EditorGame() {}

	void log(QString msg);

	bool updating() const { return mUpdating; }

signals:
	void worldUpdated();

public slots:
	void setUpdating(bool on);
	void start();
	void stop();
	void updateWorld();

private:
	static const int fps;

	QTimer* mTimer;
	QTime mElapsed;
	bool mUpdating;

	CommWindow* mComm;
};

#endif /* EDITORGAME_H_ */
