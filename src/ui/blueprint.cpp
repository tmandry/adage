#include "blueprint.h"

#include <utility>
#include <QWidget>
#include <QPainter>
#include <QSize>
#include "game.h"
#include "world/map.h"
#include "world/wall.h"

Blueprint::Blueprint(Game* game, QWidget* parent)
	:	QWidget(parent),
		mGame(game),
		mPanning(0,0),
		mZoom(1.0),
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

void Blueprint::paintEvent(QPaintEvent* /*event*/)
{
	QPainter p(this);
	
	p.scale(scale(), scale());
	p.translate(mPanning + QPoint(width()/2, height()/2) / scale());
	
	p.setPen(Qt::white);
	
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
		mZoom += event->delta() / 1200.0; //about .1 per step
		
		mZoom = std::max(mZoom, 0.1);
		mZoom = std::min(mZoom, 2.0);
		update();
	}
}

inline double Blueprint::scale() const
{
	return basePxPerFoot * mZoom;
}
