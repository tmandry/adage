#ifndef BLUEPRINT_H_
#define BLUEPRINT_H_

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include "game.h"

class Blueprint : public QWidget
{
	Q_OBJECT

public:
	Blueprint(Game* game, QWidget* parent=0);
	virtual ~Blueprint();
	
	virtual QSize sizeHint() const;
	
protected:
	void paintEvent(QPaintEvent* event);
	
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);

private:
	enum { basePxPerFoot = 2 };
	
	//returns px per foot scale with zoom
	inline double scale() const;

	Game* mGame;
	
	QPointF mPanning;
	double mZoom;
	
	bool mMovePressed;
	QPointF mMovePanningStart;
	QPoint mMoveMouseStart;
};

#endif /*BLUEPRINT_H_*/
