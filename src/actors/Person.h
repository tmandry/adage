#ifndef PERSON_H_
#define PERSON_H_

#include "Actor.h"
#include "steering/Arrive.h"
#include "world/View.h"
#include "math/Point.h"

class Person : public Actor
{
public:
	Person(Math::Point pos, Entity* parent, std::string name="Person");
	virtual ~Person() {}
	
private:
	Arrive mArrive;
};


class PersonView : public View
{
public:
	PersonView(Person* parent): mParent(parent) {}
	~PersonView() {}
	
	void paint(QPainter* p);
	
private:
	Person* mParent;
};

#endif /*PERSON_H_*/
