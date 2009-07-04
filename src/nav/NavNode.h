#ifndef NAVNODE_H_
#define NAVNODE_H_

#include <vector>
#include "math/Polygon.h"

class AStar;

class NavNode : public Math::ConvexPolygon
{
public:
	NavNode(Math::Point* begin, Math::Point* end);
	~NavNode() {}

	void setLink(int edge, NavNode* node) { mLinks[edge] = node; }

	void process(AStar* aStar);
	void discover(AStar* aStar, NavNode* caller, float arrivalCost);

	float costToNode() const { return mArrivalCost + mHeuristic; }
	int arrivalEdge() const { return mArrivalEdge; }
	NavNode* link(int idx) const { return mLinks[idx]; }

private:
	std::vector<NavNode*> mLinks;

	int mSessionId;
	bool mOpen;
	//all of these for the best-found path to this node
	float mArrivalCost;
	float mHeuristic;
	int mArrivalEdge;
};

#endif /* NAVNODE_H_ */
