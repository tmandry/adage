#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QPointF>
#include "Person.h"
#include "Ghost.h"
#include "world/World.h"
#include "steering/Arrive.h"
#include "steering/AvoidWalls.h"
#include "math/convert.h"
#include <cassert>

Person::Person(Math::Point pos, Pointer<Entity> parent, std::string name)
	:	Actor(parent, name)
{
	subclass("Person");

	setPos(pos);
	setMaxSpeed(7.0);

	mWander = new Wander(pointer(), 0.5);
	addSteeringBehavior(mWander);
	addSteeringBehavior(new AvoidWalls(pointer()));

	setView(new PersonView(pointer()));
	setVisible(true);
}

void Person::updateEvent(double secsElapsed)
{
	static ConstEntityList<Ghost> ghosts = world()->findEntities<Ghost>("Ghost");

	for (unsigned int i=0; i<mEvade.size(); ++i) {
		remSteeringBehavior(mEvade[i]);
		delete mEvade[i];
	}
	mEvade.resize(ghosts.size());

	for (unsigned int i = 0; i < ghosts.size(); ++i) {
		mEvade[i] = new Evade(pointer(), ghosts[i]);
		addSteeringBehavior(mEvade[i]);
	}

	Actor::updateEvent(secsElapsed);
}


PersonView::PersonView(Pointer<Actor> parent)
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

	/*if (mParent->inherits("Ghost")) {
		const MovingEntity* t = ((Ghost*)mParent)->mPursue.mTarget;
		QRectF icon(
			t->pos().x - 2.1, t->pos().y - 2.1,
			4.2, 4.2
		);

		p->setPen(QPen(QBrush(Qt::red), 0.6));
		p->setBrush(Qt::NoBrush);
		p->drawEllipse(icon);
	}*/
}
