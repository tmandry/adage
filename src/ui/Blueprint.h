#ifndef BLUEPRINT_H_
#define BLUEPRINT_H_

#include <QWidget>
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include <QFont>
#include <QRect>
#include "world/GameBase.h"
#include "math/Point.h"

class QPainter;

class Blueprint : public QGLWidget
{
	Q_OBJECT

public:

	Blueprint(GameBase* game, QWidget* parent=0);
	virtual ~Blueprint();

	void setGame(GameBase* game) { mGame = game; }

	float zoom() const { return mZoom; }

public slots:
	void setZoom(float zoom);
	void setShowNavmesh(int show);
	void setShowNavmesh(bool show);

signals:
	void zoomChanged(float zoom);

protected:
	virtual void paintEvent(QPaintEvent* event);

	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

	//returns px per meter scale with zoom
	float scale() const { return basePxPerMeter * mZoom; }
	//returns grid line interval in meters
	int gridResolution() const;
	QPointF panning() const { return mPanning; }
	void setPanning(QPointF panning) { mPanning = panning; }
	GameBase* game() const { return mGame; }
	QPainter* createPainter();

	Math::Point screenToWorld(QPointF point) const;

private:
	void transformPainter(QPainter* p) const;

	enum { basePxPerMeter = 2 };

	GameBase* mGame;

	QPainter* mPainter;
	QRectF mViewArea;
	QPointF mPanning;
	float mZoom;
	int mGridRes;

	bool mMovePressed;
	QPointF mMovePanningStart;
	QPoint mMoveMouseStart;

	bool mShowNavmesh;

	QFont mFont;
};

#endif /*BLUEPRINT_H_*/
