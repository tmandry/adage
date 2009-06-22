#ifndef WALL_H_
#define WALL_H_

#include <utility>
#include "world/Entity.h"
#include "world/View.h"
#include "math/Point.h"
#include "math/Vector.h"
#include "math/Segment.h"

class Wall : public Entity
{
	ENTITY(Wall)
public:
	typedef std::pair<Math::Vector, Math::Vector> NormalPair;

	Wall(Pointer<Entity> p, double x1,double y1, double x2,double y2): Entity(p,"Wall"), mSegment(Math::Point(x1,y1), Math::Point(x2,y2)) { init(); }
	Wall(Pointer<Entity> p, Math::Point a, Math::Point b): Entity(p,"Wall"), mSegment(a,b) { init(); }

	virtual ~Wall() {}

	Math::Segment segment() const { return mSegment; }

	//TODO: convenience functions that use existing functions to test for collision?

private:
	Math::Segment mSegment;
	NormalPair mNormals;

	void init();
};


class WallView : public View
{
public:
	WallView(Pointer<Wall> parent): mParent(parent) {}
	~WallView() {}

	void paint(QPainter* p);

private:
	Pointer<Wall> mParent;
};

#endif /*WALL_H_*/
