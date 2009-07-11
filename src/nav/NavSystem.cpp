#include "NavSystem.h"

int NavSystem::addNode(Math::Point* begin, Math::Point* end)
{
	mNodes.push_back(NavNode(begin, end));
	return mNodes.size();
}

void NavSystem::setLink(int from, int edge, int to)
{
	assert(from > 0 && edge > 0 && to > 0);
	mNodes.at(from-1).setLink(edge-1, &mNodes.at(to-1));
}

NavNode* NavSystem::pointToNode(Math::Point point)
{
	for (unsigned int i = 0; i < mNodes.size(); ++i)
		if (mNodes[i].contains(point)) return &mNodes[i];
	return 0;
}

bool NavSystem::findPath(NavPath& path, Math::Point start, Math::Point dest)
{
	NavNode* startNode = pointToNode(start);
	NavNode* destNode = pointToNode(dest);
	if (!startNode || !destNode) return false;

	bool result = mAStar.findPath(path, startNode, start, destNode, dest);
	if (result) smoothPath(path);
	return result;
}

bool NavSystem::isLineOfSight(Math::Point start, const NavNode* startNode, Math::Point dest, const NavNode* destNode) const
{
	Math::Segment line = Math::Segment(start, dest);
	const NavNode* node = startNode;
	NavNode::LineRelation result;

	//run until we reach the destination
	do {
		int side;
		Math::Point intersection;
		result = node->classifyLine(line, side, intersection);

		if (result == NavNode::exits) {
			node = node->link(side);
			//if node is an actual node, continue; if it is 0, we have hit a solid wall and this is not a LoS
			if (!node) return false;
		} else if (result == NavNode::none) {
			assert(!"NavNode::classifyLine returned no relation");
		}
	} while (node != destNode && result != NavNode::endsInside);

	//we made it
	return true;
}

void NavSystem::smoothPath(NavPath& path) const
{
	std::vector<NavEdge>::iterator e1, e2;
	e1 = path.edges.begin();
	e2 = e1 + 1;

	while (e2 < path.edges.end()) {
		//see if there is an extraneous edge
		if (isLineOfSight(e1->start, e1->startNode, e2->end, e2->endNode)) {
			//combine the edges and advance only the second iterator
			e1->end = e2->end;
			e1->endNode = e2->endNode;
			path.edges.erase(e2);

			e2 = e1 + 1;
		} else {
			//advance both iterators
			e1++;
			e2 = e1 + 1;
		}
	}
}
