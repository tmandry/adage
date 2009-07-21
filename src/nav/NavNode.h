#ifndef NAVNODE_H_
#define NAVNODE_H_

#include <vector>
#include "math/Polygon.h"

class AStar;

class NavNode : public Math::ConvexPolygon
{
public:
	NavNode(Math::ConvexPolygon polygon);
	~NavNode() {}

	void setLink(int edge, NavNode* node) { mLinks[edge] = node; }

	void process(AStar* aStar);
	void discover(AStar* aStar, NavNode* caller, float arrivalCost);

	enum LineRelation {
		exits, //goes through the cell
		endsInside, //point b is inside the cell
		none //line and cell don't even cross
	};
	//Determines the relationship between a path from line.a to line.b and this cell.
	LineRelation classifyLine(const Math::Segment& line, int& side, Math::Point& intersection) const;

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
