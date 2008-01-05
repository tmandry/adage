#ifndef PERSON_H_
#define PERSON_H_

#include <vector>
#include <utility>
#include <QColor>
#include "Actor.h"
#include "steering/Arrive.h"
#include "steering/Wander.h"
#include "steering/Flee.h"
#include "world/View.h"
#include "math/Point.h"

class Person : public Actor
{
public:
	Person(Math::Point pos, Entity* parent, std::string name="Person");
	virtual ~Person() {}
	
private:
	friend class PersonView;
	virtual void updateEvent(double secsElapsed);
	
	std::vector<Flee*> mFlee;
	Arrive mArrive;
	Wander mWander;
};


class PersonView : public View
{
public:
	PersonView(Actor* parent);
	~PersonView() {}
	
	void paint(QPainter* p);
	void setColor(QColor color) { mColor = color; }
	
private:
	Actor* mParent;
	QColor mColor;
};

#endif /*PERSON_H_*/
