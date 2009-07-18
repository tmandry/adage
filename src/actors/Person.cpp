#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QPointF>
#include <QBitmap>
#include "Person.h"
#include "Ghost.h"
#include "GhostBuster.h"
#include "Navigator.h"
#include "world/World.h"
#include "steering/Arrive.h"
#include "steering/AvoidWalls.h"
#include "steering/Separation.h"
#include "steering/Cohesion.h"
#include "steering/Alignment.h"
#include "math/convert.h"
#include "math/rand.h"
#include <cassert>

Person::Person(Math::Point pos, Pointer<Entity> parent, QString name)
	:	Actor(parent, name),
		mUpdateTimer(Math::randFloat(0, 0.5)) //keep from having a billion people update at once by adding a random offset
{
	subclass();

	setPos(pos);
	setMaxSpeed(7.0);

	mWander = new Wander(pointer(), 0.5);
	mEvade = new Evade(pointer());
	mFindSafety = new Arrive(pointer(), Math::Point(), Arrive::fast, 0.8, 91);
	mFindSafety->off();
	addSteeringBehavior(new AvoidWalls(pointer()));
	addSteeringBehavior(mFindSafety);
	addSteeringBehavior(mEvade);
	addSteeringBehavior(new Separation(pointer()));
	addSteeringBehavior(new Cohesion(pointer()));
	//addSteeringBehavior(new Alignment(pointer()));
	addSteeringBehavior(mWander);

	setNeighborRadius(25.0);
	setNeighborType<Person>();

	setView(new PersonView(pointer(), Qt::white));
	setVisible(true);
}

void Person::updateEvent(double secsElapsed)
{
	/*static EntityList<Ghost> ghosts = world()->findEntities<Ghost>("Ghost");

	for (unsigned int i=0; i<mEvade.size(); ++i) {
		remSteeringBehavior(mEvade[i]);
		delete mEvade[i];
	}
	mEvade.resize(ghosts.size());

	for (unsigned int i = 0; i < ghosts.size(); ++i) {
		mEvade[i] = new Evade(pointer(), ghosts[i]);
		addSteeringBehavior(mEvade[i]);
	}*/

	const double targetUpdateInterval = 0.5; //seconds

	mUpdateTimer += secsElapsed;
	if (mUpdateTimer >= targetUpdateInterval) {
		Pointer<Ghost> ghost = world()->findNearestEntity<Ghost>(pos(), 75);
		if (ghost) mEvade->setTarget(ghost);

		//find a place of relative safety.. a ghost trap
		Pointer<Entity> trap = world()->findNearestEntity<Entity>(pos(), 40);
		//don't go if we're already close by enough
		mFindSafety->off();
		if (trap) {
			double distSq = Math::distanceSq(trap->pos(), pos());
			if (distSq > 225) {
				mFindSafety->setTarget(trap->pos());
				mFindSafety->on();
				mFindSafety->setFactor(0.8 * distSq/1200);
			}
		}
	}

	Actor::updateEvent(secsElapsed);
}


PersonView::PersonView(Pointer<Actor> parent, QColor color)
	:	mParent(parent),
		mPixmap(28, 20),
		mShowName(false),
		mNameColor(color)
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
	if (mParent->inherits<Navigator>()) {
		//paint their path
		Pointer<Navigator> parent = (Pointer<Navigator>)mParent;
		NavPath& path = parent->mPath;

		if (parent->mPath.start != parent->mPath.end) {
			QPainterPath diagram;

			diagram.moveTo(path.start);
			for (unsigned int i = 0; i < path.edges.size(); ++i)
				diagram.lineTo(path.edges[i].end);
			diagram.lineTo(path.end);

			p->setPen(Qt::gray);
			p->drawPath(diagram);
		}
	}

	p->save();

	p->translate(mParent->pos().x, mParent->pos().y);
	if (mShowName) {
		p->setPen(mNameColor);
		p->drawText(QPointF(-1.3, -3), mParent->name());
	}

	p->scale(.13, .13);
	p->rotate( -Math::toDegrees(mParent->heading().absAngle()) );

	if (mParent->inherits<Navigator>()) p->scale(2.0, 2.0);

	p->drawPixmap(-14, -10, mPixmap);

	p->restore();

	//if (mParent->inherits("GhostBuster")) {
		/*const EntityList<Ghost> targets = mParent->world()->findEntities<Ghost>(mParent->pos(), 50, "Ghost");

		p->setBrush(Qt::NoBrush);
		p->setPen(QPen(QBrush(Qt::yellow), 0.1));
		p->drawEllipse(mParent->pos(), 50, 50);

		p->setPen(QPen(QBrush(Qt::darkYellow), 0));
		for (EntityList<Ghost>::const_iterator t = targets.begin(); t != targets.end(); ++t) {
			if (!*t) continue;
			p->drawEllipse((*t)->pos(), 2.4, 2.4);
		}*/

		/*p->setPen(QPen(QBrush(Qt::red), 0));
		Pointer<Ghost> target = ((Pointer<GhostBuster>)mParent)->mTarget;
		if (target) p->drawEllipse(target->pos(), 2.7, 2.7);*/
	//}

	/*if (mParent->inherits("Person")) {
		Pointer<Ghost> target = ((Pointer<Person>)mParent)->mEvade->target();
		if (target) {
			p->setPen(QPen(QBrush(Qt::gray), 0.01));
			p->drawLine(mParent->pos(), target->pos());
		}
	}*/
}

bool PersonView::clickHit(Math::Point point) const
{
	//approximate with a circle
	const double radius = 3.2;

	return (Math::distanceSq(point, mParent->pos()) < radius*radius);
}
