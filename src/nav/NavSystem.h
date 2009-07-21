#ifndef NAVSYSTEM_H_
#define NAVSYSTEM_H_

#include <vector>
#include "NavNode.h"
#include "NavPath.h"
#include "AStar.h"
#include "math/Polygon.h"

class NavSystem
{
public:
	typedef std::vector<NavNode*>::iterator NodeIterator;

	NavSystem() {}
	virtual ~NavSystem() {}

	NavNode* addNode(Math::ConvexPolygon polygon);
	void setLink(int from, int edge, int to);
	NavNode* pointToNode(Math::Point point);

	bool findPath(NavPath& path, Math::Point start, Math::Point dest);
	bool isLineOfSight(Math::Point start, const NavNode* startNode, Math::Point dest, const NavNode* destNode) const;

	NodeIterator nodesBegin() { return mNodes.begin(); }
	NodeIterator nodesEnd() { return mNodes.end(); }

private:
	void smoothPath(NavPath& path) const;

	AStar mAStar;
	std::vector<NavNode*> mNodes;
};

#endif /* NAVSYSTEM_H_ */
