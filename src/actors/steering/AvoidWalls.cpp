#include <vector>
#include "AvoidWalls.h"
#include "world/World.h"
#include "world/Wall.h"

AvoidWalls::AvoidWalls(Actor* parent)
	:	SteeringBehavior(parent, 1.0)
{

}

AvoidWalls::~AvoidWalls() {

}

Math::Vector AvoidWalls::calculate()
{
	std::vector<Math::Segment> feelers;

	Math::Vector v1 = parent()->heading() * 3.0;
	Math::Vector v2 = (v1 + parent()->heading().perpCW()*2.0) / 2.0;
	Math::Vector v3 = (v1 + parent()->heading().perpCCW()*2.0) / 2.0;
	feelers.push_back(Math::Segment(parent()->pos(), parent()->pos() + v1));
	feelers.push_back(Math::Segment(parent()->pos(), parent()->pos() + v2));
	feelers.push_back(Math::Segment(parent()->pos(), parent()->pos() + v3));

	double closestDist = 9999;
	const Wall* closestWall = 0;
	Math::Segment closestFeeler;

	ConstEntityList<Wall> walls = parent()->world()->findEntities<Wall>("Wall");

	for (std::vector<Math::Segment>::iterator feeler = feelers.begin(); feeler < feelers.end(); ++feeler) {
		for (int i = 0; i < walls.size(); ++i) {
			Math::Point point;
			double dist;
			if (Math::segmentIntersection(*feeler, walls[i]->segment(), point, dist)) {
				if (dist < closestDist) {
					closestDist = dist;
					closestWall = walls[i];
					closestFeeler = *feeler;
				}
			}
		}
	}

	if (closestWall != 0) {
		double overshoot = 3.0 - closestDist;

		Math::Vector normal = (closestWall->segment().a - closestWall->segment().b).normal().perp();
		//normal must oppose feeler direction; if not, reverse it
		if (((closestFeeler.b-closestFeeler.a) + normal).lengthSq() > lengthSq(closestFeeler)) normal *= -1;

		Math::Vector force = normal * overshoot;
		return force;
	}

	return Math::Vector();
}
