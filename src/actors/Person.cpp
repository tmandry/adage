#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include "Person.h"
#include "steering/Arrive.h"

Person::Person(Math::Point pos, Entity* parent, std::string name)
	:	Actor(parent, name),
		mArrive(this, Math::Point(50, 0), Arrive::slow)
{
	setPos(pos);
	
	setView(new PersonView(this));
	setVisible(true);
}


void PersonView::paint(QPainter* p)
{
	const float radius = 1.0; //meters
	
	Math::Point pos = mParent->pos();
	QRectF icon(pos.x-radius, pos.y-radius, radius*2, radius*2);

	p->setBrush(QBrush(Qt::white));
	p->drawEllipse(icon);
}
