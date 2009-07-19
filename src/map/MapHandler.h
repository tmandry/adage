#ifndef MAPHANDLER_H_
#define MAPHANDLER_H_

#include <map>
#include <QtXml>
#include <QString>
#include <QVariant>
#include "math/Point.h"
#include "math/Segment.h"
#include "Pointer.h"
#include "world/EntityFactory.h"

class NavSystem;
class Entity;
class NavNode;
class World;

class MapHandler : public QXmlDefaultHandler
{
public:
	MapHandler(Pointer<World> world);
	virtual ~MapHandler();

	bool startElement(const QString& namespaceURI, const QString& localName, const QString& name, const QXmlAttributes& attributes);
	bool endElement(const QString& namespaceURI, const QString& localName, const QString& name);
	bool characters(const QString& ch);
	bool fatalError(const QXmlParseException &exception);
	QString errorString() const;

private:
	bool attr(const QXmlAttributes& attributes, QString name, double& a);

	Pointer<World> mWorld;

	bool mInMap;
	bool mInNavmesh;
	bool mInNode;
	bool mInLinkList;
	bool mInEntity;
	bool mInProperty;
	bool mPropertyCompleted; //no more data should be in the property
	int mInSegment; //1 after open tag, 2 after a, 3 after b, 0 when not in a segment

	NavSystem* mNav;
	Pointer<Entity> mParentEntity;
	Pointer<Entity> mEntity;
	EntityFactory* mParentFactory;
	EntityFactory* mFactory;
	QVariant mProperty;
	QString mPropertyName;
	EntityFactory::Properties mProperties;
	Math::Point* mPoints;
	int mNumPoints;
	Math::Segment mSegment;
	QString mNodeId;
	int mLinkIdx;

	typedef std::map<QString, NavNode*> NodeMap;

	NodeMap mNodes;
	NavNode* mNode;

	QString mErrorStr;
};

#endif /* MAPHANDLER_H_ */
