#include "MapHandler.h"
#include "nav/NavSystem.h"
#include "world/Building.h"
#include "world/World.h"

MapHandler::MapHandler(Pointer<World> world)
	:	mWorld(world),
		mNav(0),
		mBuilding(0)
{
	mInMap = mInNavmesh = mInNode = mInLinkList = mInBuilding = mInWall = false;
	mPoints = new Math::Point[100];
}

MapHandler::~MapHandler() {
	delete[] mPoints;
}

bool MapHandler::startElement(const QString& /*namespaceURI*/, const QString& /*localName*/, const QString& name, const QXmlAttributes& attributes)
{
	if (!mInMap && name != "map") {
		mErrorStr = QObject::tr("The file is not a map file.");
		return false;
	}

	//top- and major-level tags
	if (name == "map") {
		QString version = attributes.value("version");
		if (!version.isEmpty() && version != "0.1") {
			mErrorStr = QObject::tr("Version mismatch: Expected Version 0.1.");
			return false;
		}

		mInMap = true;
		return true;
	} else if (name == "boundaries") {
		double left, top, right, bottom;
		if (!(
				attr(attributes, "left", left) &&
				attr(attributes, "top", top) &&
				attr(attributes, "right", right) &&
				attr(attributes, "bottom", bottom)
		)) return false;

		mWorld->setBounds(left, top, right, bottom);
		return true;
	} else if (name == "navmesh") {
		if (mNav) {
			mErrorStr = QObject::tr("More than one navmesh declared.");
			return false;
		}

		mInNavmesh = true;
		mNav = new NavSystem;
		return true;
	} else if (name == "building") {
		if (mBuilding) {
			mErrorStr = QObject::tr("Cannot nest buildings.");
			return false;
		}

		mInBuilding = true;
		mBuilding = new Building(mWorld);
		return true;
	}


	//handle common point subtag in node and wall tags
	if (name == "point" && ((mInNavmesh && mInNode) || (mInBuilding && mInWall))) {
		bool ok;

		if (attributes.value("x").isEmpty() || attributes.value("y").isEmpty()) {
			mErrorStr = QObject::tr("Both an x and y value must be specified.");
			return false;
		}

		double x = attributes.value("x").toDouble(&ok);
		if (!ok) {
			mErrorStr = QObject::tr("x value is not valid.");
			return false;
		}

		double y = attributes.value("y").toDouble(&ok);
		if (!ok) {
			mErrorStr = QObject::tr("y value is not valid.");
			return false;
		}

		mPoints[mNumPoints] = Math::Point(x, y);
		++mNumPoints;
		return true;
	}

	//navmesh subtags
	if (mInNavmesh) {
		if (name == "node") {
			if (mInNode) {
				mErrorStr = QObject::tr("Cannot nest nodes.");
				return false;
			}

			mNodeId = attributes.value("id");
			if (mNodeId.isEmpty()) {
				mErrorStr = QObject::tr("No node id specified.");
				return false;
			}

			mNumPoints = 0;
			mInNode = true;
			return true;
		}

		//node subtag
		if (mInNode) {
			//point would already be handled!
			mErrorStr = QObject::tr("Invalid tag %1 encountered inside node.").arg(name);
			return false;
		}

		if (name == "linklist") {
			mNodeId = attributes.value("id");
			if (mNodeId.isEmpty()) {
				mErrorStr = QObject::tr("No node id specified.");
				return false;
			}

			NodeMap::iterator n = mNodes.find(mNodeId);
			if (n == mNodes.end()) {
				mErrorStr = QObject::tr("Node id %1 does not exist.").arg(mNodeId);
				return false;
			}
			mNode = n->second;

			mLinkIdx = 0;
			mInLinkList = true;
			return true;
		}

		//linklist subtags (link, nolink)
		if (mInLinkList) {
			if (name == "link") {
				QString targetId = attributes.value("id");
				if (targetId.isEmpty()) {
					mErrorStr = QObject::tr("No node id specified.");
					return false;
				}

				NodeMap::iterator t = mNodes.find(targetId);
				if (t == mNodes.end()) {
					mErrorStr = QObject::tr("Node id %1 does not exist.").arg(targetId);
					return false;
				}

				mNode->setLink(mLinkIdx, t->second);
				++mLinkIdx;
				return true;
			} else if (name == "nolink") {
				//set no link.
				++mLinkIdx;
				return true;
			} else {
				mErrorStr = QObject::tr("Invalid tag %1 encountered inside linklist.").arg(name);
				return false;
			}
		}

		mErrorStr = QObject::tr("Invalid tag %1 encountered inside navmesh.").arg(name);
		return false;
	}

	//building subtags
	if (mInBuilding) {
		if (name == "wall") {
			mNumPoints = 0;
			mInWall = true;
			return true;
		}

		//wall subtag (point)
		if (mInWall) {
			//point would already be handled!
			mErrorStr = QObject::tr("Invalid tag %1 encountered inside wall.").arg(name);
			return false;
		}

		mErrorStr = QObject::tr("Invalid tag %1 encountered inside building.").arg(name);
	}

	mErrorStr = QObject::tr("Invalid tag '%1' encountered.").arg(name);
	return false;
}

bool MapHandler::endElement(const QString& /*namespaceURI*/, const QString& /*localName*/, const QString& name)
{
	if (name == "node") {
		if (mNodes.find(mNodeId) != mNodes.end()) {
			mErrorStr = QObject::tr("Node id %1 already exists.").arg(mNodeId);
			return false;
		}

		NavNode* node = mNav->addNode(mPoints, mPoints+mNumPoints);
		mNodes[mNodeId] = node;

		mInNode = false;
		return true;
	} else if (name == "linklist") {
		mInLinkList = false;
		return true;
	} else if (name == "navmesh") {
		//Navmesh is complete; assign it to world
		mWorld->setNavSystem(mNav);
		mInNavmesh = false;
		return true;
	} else if (name == "wall") {
		if (mNumPoints < 2) {
			mErrorStr = QObject::tr("Must have at least 2 points to define a wall.");
			return false;
		}

		mBuilding->createWalls(mPoints, mPoints+mNumPoints);

		mInWall = false;
		return true;
	} else if (name == "building") {
		mInBuilding = false;
		return true;
	}

	return true;
}

QString MapHandler::errorString() const
{
	return mErrorStr;
}

bool MapHandler::attr(const QXmlAttributes& attributes, QString name, double& a)
{
	QString value = attributes.value(name);
	if (value.isEmpty()) {
		mErrorStr = QObject::tr("No %1 value specified.").arg(name);
		return false;
	}

	bool ok;
	a = value.toDouble(&ok);

	if (!ok) {
		mErrorStr = QObject::tr("Invalid %1 value.").arg(name);
		return false;
	}

	return true;
}

bool MapHandler::fatalError(const QXmlParseException &exception)
{
	std::cerr << QObject::tr("Parse error at line %1, column %2:\n%3").arg(exception.lineNumber()).arg(exception.columnNumber()).arg(exception.message()).toStdString() << std::endl;
	return false;
}
