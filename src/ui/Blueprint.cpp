#include <cmath>
#include <utility>
#include <QWidget>
#include <QPainter>
#include <QSize>
#include <QSizePolicy>
#include "Blueprint.h"
#include "world/GameBase.h"
#include "world/Wall.h"
#include "math/real.h"
#include "world/GhostPortal.h"
#include "world/GhostTrap.h"
#include "nav/NavSystem.h"

Blueprint::Blueprint(GameBase* game, QWidget* parent)
	:	QGLWidget(parent),
		mGame(game),
		mPanning(0,0),
		mZoom(0.8),
		mGridRes(50),
		mMovePressed(false),
		mShowNavmesh(false),
		mFont("Arial", 3)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

Blueprint::~Blueprint()
{
}

void Blueprint::setZoom(float zoom)
{
	zoom = std::max(zoom, 0.1F);
	zoom = std::min(zoom, 5.0F);

	if (mZoom == zoom) return;

	mZoom = zoom;
	//TODO: calc gridRes based on zoom; let user override

	emit zoomChanged(zoom);
	update();
}

void Blueprint::setShowNavmesh(int show)
{
	if (show) mShowNavmesh = true;
	else mShowNavmesh = false;
	update();
}

void Blueprint::setShowNavmesh(bool show)
{
	mShowNavmesh = show;
	update();
}

void Blueprint::paintEvent(QPaintEvent* /*event*/)
{
	QPainter p(this);
	p.eraseRect(0,0, width(), height());

	//Calculate the area of the world being viewed, in meters
	mViewArea.setTopLeft(-mPanning - (QPointF(width(),height()) / 2.0) / scale());
	mViewArea.setBottomRight(-mPanning + (QPointF(width(),height()) / 2.0) / scale());
	transformPainter(&p);

	p.setPen(QColor(25, 65, 89));

	//draw gridlines (start with left bound rounded down to nearest line interval)
	for (int x=(int)Math::floorTo(mViewArea.left(), mGridRes); x <=(int)mViewArea.right(); x += mGridRes)
		p.drawLine(QPointF(x, mViewArea.top()), QPointF(x, mViewArea.bottom()));
	for (int y=(int)Math::floorTo(mViewArea.top(), mGridRes); y<=(int)mViewArea.bottom(); y += mGridRes)
		p.drawLine(QPointF(mViewArea.left(), y), QPointF(mViewArea.right(), y));

	if (!mGame) return;

	p.setFont(mFont);
	mGame->world()->paint(&p);

	NavSystem* nav = mGame->world()->navSystem();
	if (mShowNavmesh && nav) {
		//draw the navmesh for debugging/development purposes
		for (NavSystem::NodeIterator i = nav->nodesBegin(); i != nav->nodesEnd(); ++i) {
			for (unsigned int e = 0; e < (*i)->points().size(); ++e) {
				//draw linked edges in gray, unlinked ones in dark red
				if ((*i)->link(e) != 0) p.setPen(QPen(Qt::gray, 0.1));
				else p.setPen(QPen(Qt::darkRed, 0.2));

				p.drawLine((*i)->edge(e));
			}
		}
	}
}

void Blueprint::mousePressEvent(QMouseEvent* event)
{
	switch (event->button()) {
	case Qt::LeftButton:
		mMovePressed = true;
		mMoveMouseStart = event->pos();
		mMovePanningStart = mPanning;
		break;

	default:
		break;
	}
}

QPainter* Blueprint::createPainter()
{
	QPainter* p = new QPainter(this);
	transformPainter(p);
	return p;
}

void Blueprint::transformPainter(QPainter* p) const
{
	p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform/* | QPainter::HighQualityAntialiasing*/);

	//transform the drawing space based on zoom and panning
	p->scale(scale(), scale());
	p->translate(-mViewArea.topLeft());
}

void Blueprint::mouseMoveEvent(QMouseEvent* event)
{
	if (mMovePressed) {
		QPointF dist = event->pos() - mMoveMouseStart;
		dist /= scale();

		mPanning = mMovePanningStart + dist;

		update();
	}
}

void Blueprint::mouseReleaseEvent(QMouseEvent* event)
{
	switch (event->button()) {
	case Qt::LeftButton:
		mMovePressed = false;
		break;
	default:
		break;
	}
}

void Blueprint::wheelEvent(QWheelEvent* event)
{
	if (event->orientation() == Qt::Vertical) {
		//TODO: smooth zooming!
		//		motion blur?
		float z = mZoom;
		z += event->delta() / 1200.0; //about .1 per step

		setZoom(z);
	}
}

int Blueprint::gridResolution() const
{
	return 10;
}

Math::Point Blueprint::screenToWorld(QPointF point) const
{
	return Math::Point(-panning() + (-QPointF(width(),height()) / 2.0 + point) / scale());
}
