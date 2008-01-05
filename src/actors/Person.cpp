#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QPointF>
#include "Person.h"
#include "Ghost.h"
#include "world/World.h"
#include "steering/Arrive.h"
#include "math/convert.h"
#include <cassert>

Person::Person(Math::Point pos, Entity* parent, std::string name)
	:	Actor(parent, name),
		mWander(this, 0.5)
{
	subclass("Person");
	
	setPos(pos);
	setMaxSpeed(7.0);
	
	addSteeringBehavior(&mWander);
	
	setView(new PersonView(this));
	setVisible(true);
}

void Person::updateEvent(double secsElapsed)
{
	EntityList<Ghost>::const_type ghosts = world()->findEntities<Ghost>("Ghost");
	
	for (unsigned int i=0; i<mEvade.size(); ++i) {
		remSteeringBehavior(mEvade[i]);
		delete mEvade[i];
	}
	mEvade.resize(ghosts.second - ghosts.first);
	
	for (unsigned int i = 0; i < ghosts.second-ghosts.first; ++i) {
		mEvade[i] = new Evade(this, *(ghosts.first+i));
		addSteeringBehavior(mEvade[i]);
	}
	
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
	
	if (mParent->inherits("Ghost")) {
		const MovingEntity* t = ((Ghost*)mParent)->mPursue.mTarget;
		QRectF icon(
			t->pos().x - 2.1, t->pos().y - 2.1,
			4.2, 4.2
		);
		
		p->setPen(QPen(QBrush(Qt::red), 0.6));
		p->setBrush(Qt::NoBrush);
		p->drawEllipse(icon);
	}
}
