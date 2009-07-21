#ifndef NAVGEN_H_
#define NAVGEN_H_

#include <QList>
#include <cmath>
#include "math/Segment.h"
#include "math/Polygon.h"

class NavSystem;
class World;

class NavGen
{
public:
	NavGen(World* world): mWorld(world) {}
	virtual ~NavGen() {}

	void generateNavmesh();
	QList<Math::Segment> segments() const { return mSegments; }

private:
	QList<Math::ConvexPolygon> makePolygons();

	World* mWorld;
	QList<Math::Segment> mSegments;
};

inline uint qHash(const Math::Point& p)
{
	int hash = ((int)fabs((p.x+p.y)*(p.x+p.y) + p.x - p.y))/2;
	if (p.x < 0) hash += 1;
	if (p.y < 0) hash -= 3;
	return hash;
}

#endif /* NAVGEN_H_ */
