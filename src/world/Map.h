/*
 * Map.h
 *
 *  Created on: Jun 21, 2009
 *      Author: tyler
 */

#ifndef MAP_H_
#define MAP_H_

#include <string>
#include <map>
#include <QXmlDefaultHandler>
#include "Pointer.h"
#include "World.h"

class QFile;
class QTextStream;
class NavSystem;
class Building;

class Map : public QXmlDefaultHandler
{
public:
	Map(Pointer<World> parent, std::string filename = "");
	virtual ~Map();

	void open(std::string filename);
	void load();

	bool startElement(const QString& namespaceURI, const QString& localName, const QString& name, const QXmlAttributes& attributes);
	bool endElement(const QString& namespaceURI, const QString& localName, const QString& name);
	bool fatalError(const QXmlParseException &exception);
	QString errorString() const;

private:
	bool attr(const QXmlAttributes& attributes, QString name, double& a);

	Pointer<World> mParent;
	QFile* mFile;

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

#endif /* MAP_H_ */
