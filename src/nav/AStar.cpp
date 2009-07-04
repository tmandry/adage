#include "AStar.h"

bool AStar::findPath(NavPath& path, NavNode* startNode, Math::Point startPos, NavNode* endNode, Math::Point endPos)
{
	//This search starts at the end node and goes to the start node.
	mEndPos = startPos;
	//increment the session ID so nodes will know that their previous data is invalid
	++mSessionId;

	//prepare the Open list for use
	mOpen.clear();

	//add the first cell to the Open list to begin
	endNode->discover(this, 0, 0);
	while (!mOpen.empty()) {
		//pop off the top open cell (the one with the lowest cost)
		NavNode* thisNode = mOpen.back();
		mOpen.pop_back();

		if (thisNode == startNode) {
			//we're done; build the path and exit
			buildPath(path, startNode, startPos, endNode, endPos);
			return true;
		} else {
			//process the cell, adding any new neighbors to the Open heap
			thisNode->process(this);
		}
	}

	//no path exists; destination is unreachable.
	return false;
}

//Builds a waypoint list based on the path found
void AStar::buildPath(NavPath& path, NavNode* startNode, Math::Point startPos, NavNode* endNode, Math::Point endPos)
{
	path.waypoints.clear();
	path.start = startPos;
	path.startNode = startNode;
	path.end = endPos;
	path.endNode = endNode;

	//step through each linked node from startNode to endNode
	NavNode* node = startNode;
	while (node && node != endNode) {
		int edge = node->arrivalEdge();

		path.addWayPoint(node, node->edge(edge).midpoint());
		//continue on..
		node = node->link(edge);
	}
}

float AStar::computeHeuristic(NavNode* node) const
{
	return Math::distance(node->centerPoint(), mEndPos);
}
