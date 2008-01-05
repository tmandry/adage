#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QPointF>
#include "Person.h"
#include "world/World.h"
#include "steering/Arrive.h"
#include "math/convert.h"
#include <cassert>

Person::Person(Math::Point pos, Entity* parent, std::string name)
	:	Actor(parent, name),
		mArrive(this, Math::Point(50, 0), Arrive::normal),
		mWander(this, 0.8)
{
	subclass("Person");
	
	setPos(pos);
	setMaxSpeed(7.5);
	mArrive.off();
	
	setView(new PersonView(this));
	setVisible(true);
}

void Person::updateEvent(double secsElapsed)
{
	const World::EntityList& ghosts(world()->findEntities("Ghost"));
	
	for (unsigned int i=0; i<mFlee.size(); ++i) delete mFlee[i];
	mFlee.resize(ghosts.size());
	
	for (unsigned int i=0; i<ghosts.size(); ++i)
		mFlee[i] = new Flee(this, ghosts[i]->pos());
	
	Actor::updateEvent(secsElapsed);
}


PersonView::PersonView(Actor* parent)
	:	mParent(parent),
		mColor(Qt::white)
{}

void PersonView::paint(QPainter* p)
{
	static const QPointF points[] = {
			QPointF(-1.2, -0.8),
			QPointF( 1.2,  0.0),
			QPointF(-1.2,  0.8)
	};
	
	p->save();
	
	p->translate(mParent->pos());
	p->rotate( -Math::toDegrees(mParent->heading().absAngle()) );
	p->setPen(mColor);
	p->setBrush(QBrush(mColor));
	p->drawConvexPolygon(points, 3);
	
	p->restore();
}
