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

#include <iostream>
using namespace std;

bool NavSystem::findPath(NavPath& path, Math::Point start, Math::Point dest)
{
	cout << dest.x << " " << dest.y << endl;
	NavNode* startNode = pointToNode(start);
	NavNode* destNode = pointToNode(dest);
	if (!startNode || !destNode) return false;

	return mAStar.findPath(path, startNode, start, destNode, dest);
}
