#ifndef MAPGENERATOR_H_
#define MAPGENERATOR_H_

#include <map>
#include <QTextStream>
#include "math/Point.h"
#include "math/Segment.h"
#include "Pointer.h"

class World;
class NavNode;
class Building;
class Entity;

//Generates map files.
class MapGenerator
{
public:
	MapGenerator(Pointer<World> world);
	virtual ~MapGenerator();

	bool write(QIODevice* device);

private:
	static const char* versionString;

	static QString indent(int depth);

	void generateNode(NavNode* node, int depth);
	void generateLinkList(NavNode* node, int depth);
	void generateEntity(Pointer<Entity> ent, int depth);
	void generatePoint(Math::Point point, int depth);
	void generateSegment(Math::Segment segment, int depth);

	typedef std::map<NavNode*, QString> IdMap;
	IdMap mIdMap;
	int mLastId;
	Pointer<World> mWorld;
	QTextStream out;
};

#endif /* MAPGENERATOR_H_ */
