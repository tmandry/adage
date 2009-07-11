#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

#include "Actor.h"
#include "nav/NavPath.h"

class Seek;

class Navigator: public Actor
{
public:
	Navigator(Pointer<Entity> parent, std::string name = "Navigator");
	virtual ~Navigator();

	bool goTo(Math::Point dest);

private:
	friend class PersonView;

	virtual void updateEvent(double secsElapsed);
	void nextEdge();

	NavPath mPath;
	unsigned int mEdge;
	Seek* mSeek;
};

#endif /* NAVIGATOR_H_ */
