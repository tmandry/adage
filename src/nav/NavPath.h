
#ifndef NAVPATH_H_
#define NAVPATH_H_

#include "NavNode.h"

struct NavWayPoint
{
	NavWayPoint(NavNode* Node, Math::Point Destination): node(Node), destination(Destination) {}
	NavNode* node;
	Math::Point destination;
};

struct NavPath
{
	Math::Point start;
	NavNode* startNode;
	Math::Point end;
	NavNode* endNode;
	std::vector<NavWayPoint> waypoints;

	void addWayPoint(NavNode* node, Math::Point dest) { waypoints.push_back(NavWayPoint(node, dest)); }
};

#endif /* NAVPATH_H_ */
