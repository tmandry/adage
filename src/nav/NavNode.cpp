#include "NavNode.h"
#include "AStar.h"

NavNode::NavNode(Math::Point* begin, Math::Point* end)
	:	Math::ConvexPolygon(begin, end),
		mLinks(points().size(), 0),
		mSessionId(-1)
{
}

void NavNode::process(AStar* aStar)
{
	//once we've been processed, we're closed
	mOpen = false;

	//go through all neighbors to see if they need to be added to the open list
	for (unsigned int i = 0; i < mLinks.size(); ++i)
	{
		if (mLinks[i])
			mLinks[i]->discover(aStar, this, mArrivalCost+Math::distance(edge(mArrivalEdge).midpoint(), edge(i).midpoint()));
	}
}

void NavNode::discover(AStar* aStar, NavNode* caller, float arrivalCost)
{
	if (aStar->sessionId() != mSessionId) {
		//new pathfinding session; reset best-path data
		mSessionId = aStar->sessionId();

		if (caller) {
			mOpen = true;
			mHeuristic = aStar->computeHeuristic(this);
			mArrivalCost = arrivalCost;

			//find out and save which edge we are coming from
			mArrivalEdge = -1;
			for (unsigned int i = 0; i < mLinks.size(); ++i)
				if (caller == mLinks[i]) { mArrivalEdge = i; break; }
			assert(mArrivalEdge != -1);
		} else {
			//caller==0; we're the starting cell of the search
			mOpen = false;
			mArrivalCost = mHeuristic = 0;
			mArrivalEdge = 0;
		}

		aStar->addOpenNode(this);
	} else if (mOpen) {
		//We are already in the Open list; we need to check if this caller has a better (faster) path
		if (arrivalCost < mArrivalCost) {
			mArrivalCost = arrivalCost;

			//find out and save which edge we are coming from
			mArrivalEdge = -1;
			for (unsigned int i = 0; i < mLinks.size(); ++i)
				if (caller == mLinks[i]) { mArrivalEdge = i; break; }
			assert(mArrivalEdge != -1);

			//TODO update our position in the list
			aStar->updateOpenNode(this);
		}
	}
	//if the cell is closed, we do nothing.
}

NavNode::LineRelation NavNode::classifyLine(const Math::Segment& line, int& side, Math::Point& intersection) const
{
	/*//check if the line terminates inside the cell
	if (this->contains(line.b))
		return endsInside;*/

	int numInsideLines = 0; //keep track of how many sides the endpoint is "inside"

	//check if it exits the cell
	for (unsigned int i = 0; i < points().size(); ++i) {
		if (edge(i).classifyPoint(line.b) != Math::Segment::right) { //if it ends on the outside side
			if (edge(i).classifyPoint(line.a) != Math::Segment::left) { //and starts on the inside side
				//check for intersection
				double dummy;
				if (Math::segmentIntersection(line, edge(i), intersection, dummy)) {
					side = i;
					return exits;
				}
			}
		} else { //ends on the inside side
			++numInsideLines;
		}
	}

	if (numInsideLines == points().size()) //if it is inside every side
		return endsInside;

	return none;
}
