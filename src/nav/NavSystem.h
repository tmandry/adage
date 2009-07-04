#ifndef NAVSYSTEM_H_
#define NAVSYSTEM_H_

#include <vector>
#include "NavNode.h"
#include "NavPath.h"
#include "AStar.h"

class NavSystem
{
public:
	typedef std::vector<NavNode>::iterator NodeIterator;

	NavSystem() {}
	virtual ~NavSystem() {}

	int addNode(Math::Point* begin, Math::Point* end);
	void setLink(int from, int edge, int to);
	NavNode* pointToNode(Math::Point point);

	bool findPath(NavPath& path, Math::Point start, Math::Point dest);

	NodeIterator nodesBegin() { return mNodes.begin(); }
	NodeIterator nodesEnd() { return mNodes.end(); }

private:
	AStar mAStar;
	std::vector<NavNode> mNodes;
};

#endif /* NAVSYSTEM_H_ */
