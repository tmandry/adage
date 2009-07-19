#include "MapGenerator.h"
#include "world/World.h"
#include "nav/NavSystem.h"
#include "world/Building.h"
#include "world/Wall.h"
#include "world/EntityFactory.h"
#include "world/FactoryManager.h"
#include "world/Entities.h"

const char* MapGenerator::versionString = "0.2";

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
	NavSystem* nav = mWorld->navSystem();
	if (nav) {
		out << "<navmesh>\n";

		for (NavSystem::NodeIterator i = nav->nodesBegin(); i != nav->nodesEnd(); ++i)
			generateNode(*i, 1);
		out << "\n";
		for (NavSystem::NodeIterator i = nav->nodesBegin(); i != nav->nodesEnd(); ++i)
			generateLinkList(*i, 1);

		out << "</navmesh>\n\n";
	}

	//generate Entities
	Entity::ChildList entities = mWorld->_childEntities();
	foreach(Pointer<Entity> ent, entities)
		generateEntity(ent, 0);

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
	for (unsigned int i = 0; i < node->points().size(); ++i) {
		generatePoint(node->points()[i], depth+1); out << "\n";
	}
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

void MapGenerator::generateEntity(Pointer<Entity> ent, int depth)
{
	QString type = ent->_className();
	EntityFactory* factory = FactoryManager::instance()->getFactory(type);

	out << indent(depth) << "<" << type << ">\n";

	//generate all properties
	foreach(QString property, factory->getPropertyNames()) {
		QVariant value = factory->getProperty(ent, property);

		out << indent(depth+1) << "<" << property << ">";

		if (value.userType() == qMetaTypeId<Math::Point>())
			generatePoint(value.value<Math::Point>(), 0); //same line; don't indent
		else if (value.userType() == qMetaTypeId<Math::Segment>()) {
			//spread out over multiple lines
			out << "\n";
			generateSegment(value.value<Math::Segment>(), depth+2);
			out << indent(depth+1);
		} else
			out << value.toString();

		out << "</" << property << ">\n";
	}

	//once we're finished with properties, recursively generate any children the entity has
	Entity::ChildList children = ent->children();
	foreach(Pointer<Entity> child, children)
		generateEntity(child, depth+1);

	out << indent(depth) << "</" << type << ">\n";
}

void MapGenerator::generatePoint(Math::Point point, int depth)
{
	out << indent(depth) << "<Point x=\"" << point.x << "\" y=\"" << point.y << "\" />";
}

void MapGenerator::generateSegment(Math::Segment segment, int depth)
{
	out << indent(depth) << "<Segment>\n";
	generatePoint(segment.a, depth+1); out << "\n";
	generatePoint(segment.b, depth+1); out << "\n";
	out << indent(depth) << "</Segment>\n";
}
