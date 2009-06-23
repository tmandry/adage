#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QPointF>
#include <QBitmap>
#include "Person.h"
#include "Ghost.h"
#include "GhostBuster.h"
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

	setView(new PersonView(pointer(), Qt::white));
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


PersonView::PersonView(Pointer<Actor> parent, QColor color)
	:	mParent(parent),
		mPixmap(28, 20)
{
	setColor(color);
}

void PersonView::setColor(QColor color)
{
	mColor = color;

	QBitmap mask(28, 20);
	QPainter p;

	static const QPoint points[] = {
			QPoint(2,  2 ),
			QPoint(26, 10),
			QPoint(2,  18)
	};

	p.begin(&mPixmap);
	p.setPen(mColor);
	p.setBrush(QBrush(mColor));
	p.drawConvexPolygon(points, 3);
	p.end();

	mask.clear();
	p.begin(&mask);
	p.setPen(Qt::color1);
	p.setBrush(QBrush(Qt::color1));
	p.drawConvexPolygon(points, 3);
	p.end();
	mPixmap.setMask(mask);
}

void PersonView::paint(QPainter* p)
{
	p->save();

	p->translate(mParent->pos().x, mParent->pos().y);
	p->rotate( -Math::toDegrees(mParent->heading().absAngle()) );
	p->scale(.13, .13);

	p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	p->drawPixmap(-14, -10, mPixmap);

	p->restore();

	if (mParent->inherits("GhostBuster")) {
		/*const ConstEntityList<Ghost> targets = mParent->world()->findEntities<Ghost>(mParent->pos(), 50, "Ghost");

		p->setBrush(Qt::NoBrush);
		p->setPen(QPen(QBrush(Qt::yellow), 0.1));
		//p->drawEllipse(mParent->pos(), 50, 50);

		p->setPen(QPen(QBrush(Qt::darkYellow), 0));
		for (ConstEntityList<Ghost>::const_iterator t = targets.begin(); t != targets.end(); ++t) {
			if (!*t) continue;
			p->drawEllipse((*t)->pos(), 2.4, 2.4);
		}*/

		p->setPen(QPen(QBrush(Qt::red), 0));
		Pointer<Ghost> target = ((Pointer<GhostBuster>)mParent)->mTarget;
		if (target) p->drawEllipse(target->pos(), 2.7, 2.7);
	}
}
