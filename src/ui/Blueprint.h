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
	
	float zoom() const { return mZoom; }

public slots:
	void setZoom(float zoom);
	//eventually will be used for returning the view to the main point of interest in the mission
	void goHome();

signals:
	void zoomChanged(float zoom);
	
protected:
	void paintEvent(QPaintEvent* event);
	
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);

private:
	enum { basePxPerMeter = 2 };
	
	//returns px per meter scale with zoom
	inline float scale() const;
	//returns grid line interval in meters
	int gridResolution() const;

	Game* mGame;
	
	QPointF mPanning;
	float mZoom;
	int mGridRes;
	
	bool mMovePressed;
	QPointF mMovePanningStart;
	QPoint mMoveMouseStart;
};

#endif /*BLUEPRINT_H_*/
