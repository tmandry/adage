#ifndef BLUEPRINT_H_
#define BLUEPRINT_H_

#include <QWidget>
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include "Game.h"

class Blueprint : public QWidget
{
	Q_OBJECT

public:
	enum ToolType {
		dormantPortal,
		portal,
		trap,
		nav
	};

	Blueprint(Game* game, QWidget* parent=0);
	virtual ~Blueprint();

	float zoom() const { return mZoom; }

public slots:
	void setZoom(float zoom);
	void setTool(int tool);
	void setShowNavmesh(int show);
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

	void placeTool(Math::Point pos);

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

	ToolType mTool;
	bool mShowNavmesh;
};

#endif /*BLUEPRINT_H_*/
