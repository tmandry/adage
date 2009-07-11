
#ifndef NAVPATH_H_
#define NAVPATH_H_

#include "NavNode.h"

struct NavEdge
{
	NavEdge(NavNode* StartNode, Math::Point Start, NavNode* EndNode, Math::Point End): startNode(StartNode), start(Start), endNode(EndNode), end(End) {}
	NavNode* startNode;
	Math::Point start;
	NavNode* endNode;
	Math::Point end;

	//any extra edge traversal information can go here!
};

struct NavPath
{
	Math::Point start;
	NavNode* startNode;
	Math::Point end;
	NavNode* endNode;
	std::vector<NavEdge> edges;

	void addEdge(NavNode* startNode, Math::Point start, NavNode* endNode, Math::Point end) { edges.push_back(NavEdge(startNode, start, endNode, end)); }
};

#endif /* NAVPATH_H_ */
