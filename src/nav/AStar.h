#ifndef ASTAR_H_
#define ASTAR_H_

#include <vector>
#include <algorithm>
#include "NavNode.h"
#include "NavPath.h"

class AStar
{
private:
	class OpenNavNodeSort
	{
	public:
		bool operator() (const NavNode* lhs, const NavNode* rhs) const throw()
		{
			return lhs->costToNode() > rhs->costToNode();
		}
	};

public:
	AStar(): mSessionId(0) {}
	virtual ~AStar() {}

	int sessionId() const { return mSessionId; }

	bool findPath(NavPath& path, NavNode* startNode, Math::Point startPos, NavNode* endNode, Math::Point endPos);

	void addOpenNode(NavNode* node)
	{
		mOpen.insert(
			std::upper_bound(mOpen.begin(), mOpen.end(), node, OpenNavNodeSort()),
			node
		);
	}
	void updateOpenNode(NavNode* node)
	{
		mOpen.erase(std::find(mOpen.begin(), mOpen.end(), node));
		addOpenNode(node);
	}
	float computeHeuristic(NavNode* node) const;

private:
	void buildPath(NavPath& path, NavNode* startNode, Math::Point startPos, NavNode* endNode, Math::Point endPos);

	int mSessionId; //used to keep track of different pathfinding sessions

	std::vector<NavNode*> mOpen;
	Math::Point mEndPos;
};

#endif /* ASTAR_H_ */
