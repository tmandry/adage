#include <QPen>
#include <QPainter>
#include "Wall.h"

void Wall::init()
{
	Math::Vector temp = mA-mB;
	temp.normalize();
	
	mNormals.first = temp.perpCW();
	mNormals.second = temp.perpCCW();
	
	setView(new WallView(this));
	setVisible(true);
}

void WallView::paint(QPainter* p)
{
	QPen wallPen(Qt::white);
	p->setPen(wallPen);
	
	p->drawLine(mParent->segment());
}
