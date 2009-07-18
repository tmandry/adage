#include "MapGenerator.h"
#include "world/World.h"
#include "nav/NavSystem.h"
#include "world/Building.h"
#include "world/Wall.h"

const char* MapGenerator::versionString = "0.1";

MapGenerator::MapGenerator(Pointer<World> world)
	:	mLastId(0),
		mWorld(world)
{
}

MapGenerator::~MapGenerator()
{
}

bool MapGenerator::write(QIODevice* device)
{
	out.setDevice(device);
	out.setCodec("UTF-8");
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<!DOCTYPE map>\n"
		<< "<map version=\"" << versionString << "\">\n";

	//output boundaries
	out << "<boundaries left=\"" << mWorld->leftBound() << "\" top=\"" << mWorld->topBound() << "\" right=\"" << mWorld->rightBound() << "\" bottom=\"" << mWorld->bottomBound() << "\" />\n";

	//generate navmesh
	out << "<navmesh>\n";

	NavSystem* nav = mWorld->navSystem();
	for (NavSystem::NodeIterator i = nav->nodesBegin(); i != nav->nodesEnd(); ++i)
		generateNode(*i, 1);
	out << "\n";
	for (NavSystem::NodeIterator i = nav->nodesBegin(); i != nav->nodesEnd(); ++i)
		generateLinkList(*i, 1);

	out << "</navmesh>\n\n";

	//generate buildings
	EntityList<Building> buildings = mWorld->findEntities<Building>();
	for (unsigned int i = 0; i < buildings.size(); i++)
		generateBuilding(buildings[i].pointer(), 0);

	out << "</map>\n";
	return true;
}

QString MapGenerator::indent(int depth)
{
	const int indentSize = 4;
	return QString(indentSize * depth, ' ');
}

void MapGenerator::generateNode(NavNode* node, int depth)
{
	++mLastId;
	QString id;
	QTextStream(&id) << mLastId;
	mIdMap[node] = id;

	out << indent(depth) << "<node id=\"" << id << "\">\n";
	for (unsigned int i = 0; i < node->points().size(); ++i)
		generatePoint(node->points()[i], depth+1);
	out << indent(depth) << "</node>\n";
}

void MapGenerator::generateLinkList(NavNode* node, int depth)
{
	out << indent(depth) << "<linklist id=\"" << mIdMap[node] << "\">\n";

	for (unsigned int i = 0; i < node->points().size(); ++i) {
		NavNode* link = node->link(i);
		if (link)
			out << indent(depth+1) << "<link id=\"" << mIdMap[link] << "\" />\n";
		else
			out << indent(depth+1) << "<nolink/>\n";
	}

	out << indent(depth) << "</linklist>\n";
}

void MapGenerator::generateBuilding(Building* building, int depth)
{
	out << indent(depth) << "<building>\n";

	for (unsigned int w = 0; w < building->walls().size(); ++w) {
		out << indent(depth+1) << "<wall>\n";
		generatePoint(building->walls()[w]->segment().a, depth+2);
		generatePoint(building->walls()[w]->segment().b, depth+2);
		out << indent(depth+1) << "</wall>\n";
	}

	out << indent(depth) << "</building>\n";
}

void MapGenerator::generatePoint(Math::Point point, int depth)
{
	out << indent(depth) << "<point x=\"" << point.x << "\" y=\"" << point.y << "\" />\n";
}
