#include "MapHandler.h"
#include "nav/NavSystem.h"
#include "world/World.h"
#include "world/Entities.h"
#include "world/EntityFactory.h"
#include "world/FactoryManager.h"

MapHandler::MapHandler(Pointer<World> world)
	:	mWorld(world),
		mNav(0),
		mEntity(0),
		mParentFactory(0)
{
	mInMap = mInNavmesh = mInNode = mInLinkList = mInEntity = mInProperty = false;
	mInSegment = 0;
	//mPoints = new Math::Point[100];
	mParentEntity = world;
}

MapHandler::~MapHandler() {
	//delete[] mPoints;
}

#include <iostream>
using namespace std;

bool MapHandler::startElement(const QString& /*namespaceURI*/, const QString& /*localName*/, const QString& name, const QXmlAttributes& attributes)
{
	QList<QString> entityTypes = FactoryManager::instance()->types();

	if (!mInMap && name != "map") {
		mErrorStr = QObject::tr("The file is not a map file.");
		return false;
	}

	//top- and major-level tags
	if (name == "map") {
		QString version = attributes.value("version");
		if (!version.isEmpty() && version != "0.2") {
			mErrorStr = QObject::tr("Version mismatch: Expected Version 0.2.");
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
	} else if (entityTypes.contains(name)) { //tag is an Entity
		if (mInEntity) {
			//check if we have created this entity yet or not - reaching a child means it's time to build it
			if (!mEntity) {
				mEntity = mFactory->buildEntity(mProperties, mParentEntity);
				if (mParentFactory) mParentFactory->addChild(mParentEntity, mEntity);
			}
			mProperties.clear();
			mParentEntity = mEntity;
		}

		mFactory = FactoryManager::instance()->getFactory(name);

		mInEntity = true;
		mEntity.release();
		return true;
	}


	//handle common point subtag
	if (name == "Point" && ((mInNavmesh && mInNode) || (mInEntity && mInProperty) || mInSegment)) {
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

		if (mInNavmesh) {
			mPoints.append(Math::Point(x,y));
		} else if (mInSegment) {
			if (mInSegment == 1) mSegment.a = Math::Point(x, y);
			else if (mInSegment == 2) mSegment.b = Math::Point(x, y);
			else {
				mErrorStr = QObject::tr("Extraneous Point inside Segment.");
				return false;
			}
			++mInSegment;
		} else {
			mProperty = QVariant::fromValue<Math::Point>(Math::Point(x, y));
			mPropertyCompleted = true;
		}
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

			mPoints.clear();
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

	//entity subtags (must be a property at this point)
	if (mInEntity) {
		if (mInProperty) {
			if (name == "Segment") {
				if (mInSegment) {
					mErrorStr = QObject::tr("Segment tag nested inside another Segment.");
					return false;
				}

				mInSegment = 1;
				mSegment = Math::Segment();
				return true;
			}

			//Point would already be handled!
			mErrorStr = QObject::tr("Invalid tag %1 encountered inside property %2.").arg(name).arg(mPropertyName);
			return false;
		}

		QSet<QString> properties = mFactory->getPropertyNames();
		if (properties.contains(name)) {
			if (mProperties.contains(name)) {
				mErrorStr = QObject::tr("Property %1 redefined.").arg(name);
				return false;
			}

			mInProperty = true;
			mPropertyCompleted = false;
			mPropertyName = name;
			mProperty.clear();
			return true;
		}

		mErrorStr = QObject::tr("Invalid tag %1 encountered inside entity.").arg(name);
		return false;
	}

	mErrorStr = QObject::tr("Invalid tag '%1' encountered.").arg(name);
	return false;
}

bool MapHandler::endElement(const QString& /*namespaceURI*/, const QString& /*localName*/, const QString& name)
{
	QList<QString> entityTypes = FactoryManager::instance()->types();

	if (name == "node") {
		if (mNodes.find(mNodeId) != mNodes.end()) {
			mErrorStr = QObject::tr("Node id %1 already exists.").arg(mNodeId);
			return false;
		}

		NavNode* node = mNav->addNode(mPoints);
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
	} else if (name == "Segment") {
		if (mInSegment != 3) { //too many segments has already been handled.
			mErrorStr = QObject::tr("Not enough Points in Segment.");
			return false;
		}

		mProperty = QVariant::fromValue<Math::Segment>(mSegment);
		mPropertyCompleted = true;
		mInSegment = 0;
	} else if (name == mPropertyName) {
		//make sure we haven't built the entity yet
		if (mEntity) {
			mErrorStr = QObject::tr("Entity properties must go before children.");
			return false;
		}

		//the property will be taken care of whenever the entity is built
		mProperties[name] = mProperty;
		mInProperty = false;
	} else if (entityTypes.contains(name)) {
		assert(mInEntity);
		//check if we created the entity yet
		if (!mEntity) {
			mEntity = mFactory->buildEntity(mProperties, mParentEntity);
			if (mParentFactory) mParentFactory->addChild(mParentEntity, mEntity);
		}
		mProperties.clear();
		assert(mEntity->_className() == name);

		//move back up a level
		mEntity = mParentEntity;
		if (mEntity.pointer() == mWorld.pointer()) {
			mInEntity = false;
			mParentFactory = 0;
		} else { //otherwise leave true
			mParentEntity = mParentEntity->parent();
			mParentFactory = FactoryManager::instance()->getFactory(mParentEntity->_className());
		}
	}

	return true;
}

bool MapHandler::characters(const QString& ch)
{
	QString data = ch.trimmed();
	if (data.isEmpty()) return true;

	if (!mInProperty || mPropertyCompleted) {
		mErrorStr = QObject::tr("Unexpected data found: %1").arg(data);
		return false;
	}

	assert(mProperty.isNull() || mProperty.canConvert(QVariant::String));
	mProperty = mProperty.toString() + data;
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
