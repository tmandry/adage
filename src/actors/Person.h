#ifndef PERSON_H_
#define PERSON_H_

#include <vector>
#include <utility>
#include <QColor>
#include <QPixmap>
#include "Actor.h"
#include "steering/Arrive.h"
#include "steering/Wander.h"
#include "steering/Evade.h"
#include "world/View.h"
#include "math/Point.h"

class Person : public Actor
{
	ENTITY(Person)
public:
	Person(Math::Point pos, Pointer<Entity> parent, std::string name="Person");
	virtual ~Person() {}

private:
	friend class PersonView;
	virtual void updateEvent(double secsElapsed);

	double mUpdateTimer;

	std::vector<Evade*> mEvade;
	Wander* mWander;
};


class QPixmap;

class PersonView : public View
{
public:
	PersonView(Pointer<Actor> parent, QColor color = Qt::white);
	virtual ~PersonView() {}

	void paint(QPainter* p);
	void setColor(QColor color);

private:
	Pointer<Actor> mParent;
	QColor mColor;
	QPixmap mPixmap;
};

#endif /*PERSON_H_*/
