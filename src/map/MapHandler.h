#ifndef MAPHANDLER_H_
#define MAPHANDLER_H_

#include <map>
#include <QtXml>
#include <QString>
#include "math/Point.h"
#include "Pointer.h"

class NavSystem;
class Building;
class NavNode;
class World;

class MapHandler : public QXmlDefaultHandler
{
public:
	MapHandler(Pointer<World> world);
	virtual ~MapHandler();

	bool startElement(const QString& namespaceURI, const QString& localName, const QString& name, const QXmlAttributes& attributes);
	bool endElement(const QString& namespaceURI, const QString& localName, const QString& name);
	bool fatalError(const QXmlParseException &exception);
	QString errorString() const;

private:
	bool attr(const QXmlAttributes& attributes, QString name, double& a);

	Pointer<World> mWorld;

	bool mInMap;
	bool mInNavmesh;
	bool mInNode;
	bool mInLinkList;
	bool mInBuilding;
	bool mInWall;

	NavSystem* mNav;
	Building* mBuilding;
	Math::Point* mPoints;
	int mNumPoints;
	QString mNodeId;
	int mLinkIdx;

	typedef std::map<QString, NavNode*> NodeMap;

	NodeMap mNodes;
	NavNode* mNode;

	QString mErrorStr;
};

#endif /* MAPHANDLER_H_ */
