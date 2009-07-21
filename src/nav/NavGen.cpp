#include "NavGen.h"
#include <QSet>
#include <QList>
#include <utility>
#include <cassert>
#include <cmath>
#include "world/World.h"
#include "world/Wall.h"
#include "math/Point.h"
#include "math/Segment.h"
#include "math/Vector.h"
#include "nav/NavSystem.h"
#include <iostream>

using namespace Math;

void NavGen::generateNavmesh()
{
	QList<ConvexPolygon> polygons = makePolygons();
	foreach (ConvexPolygon polygon, polygons) {
		mWorld->navSystem()->addNode(polygon);
	}

	//now connect nodes that share edges with other nodes (slow?)
	EntityList<Wall> walls = mWorld->findEntities<Wall>();
	for (NavSystem::NodeIterator i = mWorld->navSystem()->nodesBegin(); i != mWorld->navSystem()->nodesEnd(); ++i) {
		for (int edgeI = 0; edgeI < (*i)->points().size(); ++edgeI) {
			//if this edge is already linked, skip it
			if ((*i)->link(edgeI)) continue;

			//if this edge happens to be on a wall, skip it
			bool onWall = false;
			foreach (const Pointer<Wall> wall, walls)
				if (wall->segment() == (*i)->edge(edgeI)) { onWall = true; break; }
			if (onWall) continue;

			//otherwise look for an edge to match it
			bool foundEdge = false;
			for (NavSystem::NodeIterator j = i+1; j != mWorld->navSystem()->nodesEnd(); ++j) {
				for (int edgeJ = 0; edgeJ < (*j)->points().size(); ++edgeJ) {
					if (!(*j)->link(edgeJ) && (*i)->edge(edgeI) == (*j)->edge(edgeJ)) {
						(*i)->setLink(edgeI, &(**j));
						(*j)->setLink(edgeJ, &(**i));
						foundEdge = true;
						break;
					}
				}
				if (foundEdge) break;
			}

			if (!foundEdge) {
				Math::Segment edge = (*i)->edge(edgeI);
				std::cout << QString("could not match: (%1, %2) to (%3, %4).").arg(edge.a.x).arg(edge.a.y).arg(edge.b.x).arg(edge.b.y).toStdString() << std::endl;
			}
		}
	}
}

QList<ConvexPolygon> NavGen::makePolygons()
{
	QSet<Point> points;
	//first generate a set of points from every Wall in the world (QSet will ensure no duplicates)
	EntityList<Wall> walls = mWorld->findEntities<Wall>();
	foreach (Pointer<Wall> wall, walls) {
		points << wall->segment().a;
		points << wall->segment().b;
	}

	//the next part of the algorithm generates segments representing each line of sight
	//between these points. the segments are split so that they do not cross, but instead
	//come together at common points, to make building polygons from these points easier.
	QList<Segment> segments;
	int segmentCount = 0;

	//go through every pair of points and check for LoS
	for (QSet<Point>::iterator i = points.begin(); i != points.end(); ++i) {
		for (QSet<Point>::iterator j = i+1; j != points.end(); ++j) {
			Point a = *i;
			Point b = *j;
			Segment segment(a,b);
			bool los = true;

			//check every wall for intersect
			foreach (Pointer<Wall> wall, walls) {
				Segment wallSegment = wall->segment();
				Point intersection;
				double dummy;
				//make sure neither point is from this wall (otherwise we'll get a positive that doesn't count)
				if (a != wallSegment.a && a != wallSegment.b && b != wallSegment.a && b != wallSegment.b) {
					if (segmentIntersection(segment, wall->segment(), intersection, dummy)) {
						los = false;
						break;
					}
				}
			}

			if (los) {
				//the segment passed the line of sight test.
				//now we have to split it up at every intersection
				//it has with any segments that have already been added.

				//we may be splitting this segment several times, so keep a list of split-points, in order, so we can generate all the new segments easily
				typedef QList<std::pair<Point, double> > SplitPointList;
				SplitPointList splitPoints; //stores the point and its distance from segment.a (for sorting)

				splitPoints << std::make_pair(segment.a, (double)0.0);
				splitPoints << std::make_pair(segment.b, maxDouble); //don't need to calculate the actual distance, it just has to be sorted last

				bool duplicateFound = false;
				//look for intersections.
				//anything that crosses us will be split and one of its new parts will added to newSplits
				//(the other part will replace the old one). this is to avoid invalidating any iterators.
				QList<Segment> newSplits;
				for (QList<Segment>::iterator s = segments.begin(); s < segments.end(); ++s) {
					Segment testSegment(*s);
					//if it shares a point with the segment, don't test for an intersection
					if (segment.a == testSegment.a || segment.a == testSegment.b || segment.b == testSegment.a || segment.b == testSegment.b) {
						//assert(segment != testSegment && "Should never process the same segment twice");
						if (segment == testSegment) { duplicateFound = true; break; }
						continue;
					}

					Math::Point intersection;
					double distFromA;
					if (segmentIntersection(segment, testSegment, intersection, distFromA)) {
						//add this point as a splitPoint in the correct position
						for (SplitPointList::iterator p = splitPoints.begin(); p != splitPoints.end(); ++p) {
							if (p->second > distFromA) {
								splitPoints.insert(p, std::make_pair(intersection, distFromA)); //insert before first point that's farther
								break;
							}
						}

						//unless our intersection landed on its endpoint, we need to split the testSegment too -
						//this time a simple two-way split
						if (intersection != testSegment.a && intersection != testSegment.b) {
							//replace the old with /one of/ the new
							*s = Segment(testSegment.a, intersection);
							//add the other to newSplits
							newSplits << Segment(intersection, testSegment.b);
						}
					}
				}
				if (duplicateFound) continue;

				//now that we're not iterating through segments, we can add to it.
				segments << newSplits;

				//we've successfully split the segment into all its necessary parts - now generate the actual segments from the splitPoints
				for (unsigned int i = 0; i < splitPoints.size()-1; ++i)
					segments << Segment(splitPoints[i].first, splitPoints[i+1].first);

				//segments << segment;

				//the line is now fully a part of the segments list.
				++segmentCount;
			}
		}
	}

	std::cout << "segments before splits: " << segmentCount << "\n";
	std::cout << "total split segments: " << segments.size() << std::endl;

	//mSegments = segments;

	//now build a data structure keyed by a point containing each other point it's connected to by segments.
	typedef QHash<Point, QList<Point> > PointNet;
	PointNet pointNet;
	int numRemoved = 0;
	foreach (Segment segment, segments) {
		if (segment.a == segment.b) { numRemoved++; continue; }
		pointNet[segment.a].append(segment.b);
		pointNet[segment.b].append(segment.a);
	}

	std::cout << "removed " << numRemoved << std::endl;

	int numWraparounds = 0;
	int numDeadEnds = 0;
	int numGiveUps = 0;

	//now go through each point, tracing out convex polygons!
	QList<ConvexPolygon> polygons;
	for (PointNet::iterator p = pointNet.begin(); p != pointNet.end(); ++p) {
		Point startPoint = p.key();
		while (!p.value().isEmpty()) {
			//we have our starting point, pick the next point as the first point on p's list (it will be removed from the list.)
			typedef QList<Point> PointList;
			typedef QList<std::pair<PointList*, Point> > RemovalList;
			RemovalList removals; //keep track of what to remove so we can do it once we find a polygon (if we do)
			PointList* aList = &p.value();
			Point a = startPoint;
			Point b = aList->first();
			//aList->removeFirst(); //remove this point so we don't process it again (even if we don't make a polygon out of it)
			removals << std::make_pair(aList, b);

			QList<Point> polygon;
			bool makePoly = true;
			polygon << a;
			//keep adding points until we arrive back at the start
			while (b != startPoint) {
				if (polygon.contains(b)) { numWraparounds++; while (polygon.first() != b) { polygon.removeFirst(); removals.removeFirst(); } break; }
				polygon << b;

				//now pick the next point to go to - this point should be the "most clockwise" point from this one
				Segment lastSegment(a, b);
				a = b;
				aList = &pointNet[a];
				if (aList->isEmpty()) { ++numDeadEnds; makePoly = false; /*pointNet[lastSegment.a].removeOne(a);*/ break; }
				//search all the points that are "right of" this segment for the one with the smallest angle from this segment.
				//save the inLine one too in case there aren't any points to the right.
				bool rightOfFound = false;
				bool inLineFound = false;
				Point mostClockwise;
				double mostClockwiseAngle;
				Point inLine;
				foreach (Point point, *aList) {
					if (point == lastSegment.a) continue;

					Segment::PointRelation result = lastSegment.classifyPoint(point);
					if (result == Segment::right) {
						double angle = (lastSegment.b-lastSegment.a).normal().angle( (a-point).normal() ); //using Vector math
						if (!rightOfFound || angle < mostClockwiseAngle) {
							rightOfFound = true;
							mostClockwise = point;
							mostClockwiseAngle = angle;
						}
					}
					else if (result == Segment::inLine) {
						inLine = point;
						inLineFound = true;
					}
				}

				if (rightOfFound) {
					//found our next point.
					b = mostClockwise;
				/*} else if (inLineFound) {
					//only point we can go to from this one is straight ahead on the same line,
					//meaning the last endpoint we added was unnecessary and should be removed.
					polygon.removeLast();
					b = inLine;*/
				} else {
					//we can't make a polygon.
					++numGiveUps;
					makePoly = false;
					break;
				}

				//remove this point/segment from the list
				removals << std::make_pair(aList, b);
			}

			if (makePoly) {
				//we hit our starting point again and have a complete polygon.
				//do removals.
				for (RemovalList::iterator i = removals.begin(); i != removals.end(); ++i)
					(i->first)->removeOne(i->second);

				if (polygon.size() > 2) polygons << ConvexPolygon(polygon);
			} else {
				//hit a dead end or something - at least remove the first segment processed so we don't get stuck in an infinite loop
				removals.last().first->removeOne(removals.last().second);
			}
		}
	}

	mSegments = segments;

	std::cout << numWraparounds << " wraparounds, " << numGiveUps << " give ups, and " << numDeadEnds << " dead ends.\n";

	std::cout << "polygons: " << polygons.size() << std::endl;
	return polygons;
}
