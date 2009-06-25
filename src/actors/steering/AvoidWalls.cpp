#include <vector>
#include "AvoidWalls.h"
#include "world/World.h"
#include "world/Wall.h"

AvoidWalls::AvoidWalls(Pointer<Actor> parent)
	:	SteeringBehavior(parent, 1.0, 100),
		mCycle(0),
		mResults(3)
{

}

AvoidWalls::~AvoidWalls() {

}

Math::Vector AvoidWalls::calculate()
{
	//std::vector<Math::Segment> feelers;

	Math::Vector v;
	double len;
	switch (mCycle) {
		case 0: v = parent()->heading() * 5.0; len = 5.0; break;
		case 1: v = (parent()->heading()*1.949 + parent()->heading().perpCW()*1.125); len = 2.25; break;
		case 2: v = (parent()->heading()*1.949 + parent()->heading().perpCCW()*1.125); len = 2.25; break;
		default: break;
	}

	Math::Segment feeler(parent()->pos(), parent()->pos() + v);
	/*feelers.push_back(Math::Segment(parent()->pos(), parent()->pos() + v1));
	feelers.push_back(Math::Segment(parent()->pos(), parent()->pos() + v2));
	feelers.push_back(Math::Segment(parent()->pos(), parent()->pos() + v3));*/

	double closestDist = 9999;
	Pointer<const Wall> closestWall;

	static ConstEntityList<Wall> walls = parent()->world()->findEntities<Wall>("Wall");

	for (unsigned int i = 0; i < walls.size(); ++i) {
		Math::Point point;
		double dist;
		if (Math::segmentIntersection(feeler, walls[i]->segment(), point, dist)) {
			if (dist < closestDist) {
				closestDist = dist;
				closestWall = walls[i];
			}
		}
	}

	if (closestWall) {
		//--mCycle;
		double overshoot = len - closestDist;

		Math::Vector normal = (closestWall->segment().a - closestWall->segment().b).normal().perp();
		//normal must oppose feeler direction; if not, reverse it
		if (((feeler.b-feeler.a) + normal).lengthSq() > lengthSq(feeler)) normal *= -1;

		Math::Vector force = normal * overshoot;
		mResults[mCycle] = force;
	} else
		mResults[mCycle] = Math::Vector();

	if (++mCycle > 2) mCycle = 0;
	return mResults[0] + mResults[1] + mResults[2];
}
