#include "blueprint.h"

#include <cmath>
#include <utility>
#include <QWidget>
#include <QPainter>
#include <QSize>
#include "game.h"
#include "world/map.h"
#include "world/wall.h"
#include "math/real.h"

Blueprint::Blueprint(Game* game, QWidget* parent)
	:	QWidget(parent),
		mGame(game),
		mPanning(0,0),
		mZoom(1.0),
		mGridRes(10),
		mMovePressed(false)
{
	resize(320,240);
}

Blueprint::~Blueprint()
{
}

QSize Blueprint::sizeHint() const
{
	return QSize(480,640);
}

void Blueprint::setZoom(float zoom)
{	
	if (mZoom == zoom) return;
	
	mZoom = zoom;
	//TODO: calc gridRes based on zoom; let user override
	
	emit zoomChanged(zoom);
	update();
}

void Blueprint::paintEvent(QPaintEvent* /*event*/)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	
	//Calculate the area of the world being viewed, in meters
	QRectF viewArea(
		-mPanning - (QPointF(width(),height()) / 2.0) / scale(),
		-mPanning + (QPointF(width(),height()) / 2.0) / scale()
	);
	
	//transform the drawing space based on zoom and panning
	p.scale(scale(), scale());
	p.translate(-viewArea.topLeft());
	
	p.setPen(QColor(41, 105, 142));
	
	//draw gridlines (start with left bound rounded down to nearest line interval)
	for (int x=(int)Math::floorTo(viewArea.left(), mGridRes); x <=(int)viewArea.right(); x += mGridRes)
		p.drawLine(QPointF(x, viewArea.top()), QPointF(x, viewArea.bottom()));
	for (int y=(int)Math::floorTo(viewArea.top(), mGridRes); y<=(int)viewArea.bottom(); y += mGridRes)
		p.drawLine(QPointF(viewArea.left(), y), QPointF(viewArea.right(), y));
	
	QPen wallPen(Qt::white);
	p.setPen(wallPen);
	
	const Map::WallList& walls( mGame->map().walls() );
	for (unsigned int i=0; i<walls.size(); ++i)
		p.drawLine(walls[i].segment());
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
		
		z = std::max(z, 0.1F);
		z = std::min(z, 2.0F);
		
		setZoom(z);
	}
}

inline float Blueprint::scale() const
{
	return basePxPerMeter * mZoom;
}

int Blueprint::gridResolution() const
{
	
	return 10;
}
