#include <QPen>
#include <QPainter>
#include "Wall.h"

void Wall::init()
{
	subclass();

	if (mSegment.a != mSegment.b) {
		Math::Vector temp = mSegment.a-mSegment.b;
		temp.normalize();

		mNormals.first = temp.perpCW();
		mNormals.second = temp.perpCCW();
	}

	setView(new WallView(pointer()));
	setVisible(true);
}

void Wall::setSegment(Math::Segment segment)
{
	mSegment = segment;

	Math::Vector temp = mSegment.a-mSegment.b;
	temp.normalize();

	mNormals.first = temp.perpCW();
	mNormals.second = temp.perpCCW();
}

void WallView::paint(QPainter* p)
{
	if (!mParent->visible()) return;

	QPen wallPen(Qt::white);
	p->setPen(wallPen);

	p->drawLine(mParent->segment());
}

bool WallView::clickHit(Math::Point point) const
{
	return (mParent->segment().pointDistance(point) < 3.0);
}


Pointer<Entity> WallFactory::construct(Properties properties, Pointer<Entity> parent) const
{
	if (properties.contains("segment"))
		return (new Wall(properties["segment"].value<Math::Segment>(), parent))->pointer();
	//TODO: error
}

QSet<QString> WallFactory::getPropertyNames() const
{
	static QSet<QString> names;
	if (names.isEmpty()) {
		names = SimpleEntityFactory<Wall>::getPropertyNames();
		names << "segment";
	}
	return names;
}

QVariant WallFactory::getProperty(Pointer<Wall> e, QString name) const
{
	if (name == "segment") {
		return QVariant::fromValue<Math::Segment>(e->segment());
	}
	else return QVariant();
	//TODO: error
}

void WallFactory::setProperty(Pointer<Wall> e, QString name, QVariant value) const
{
	if (name == "segment") {
		e->setSegment(value.value<Math::Segment>());
	}
	//TODO: error
}
