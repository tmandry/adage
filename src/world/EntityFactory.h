#ifndef ENTITYFACTORY_H_
#define ENTITYFACTORY_H_

#include <QMap>
#include <QVariant>
#include <QString>
#include <QSet>
#include <QMetaType>
#include "Pointer.h"
#include "math/Point.h"
#include "world/Entity.h"

Q_DECLARE_METATYPE(Math::Point)

//The base class for all Entity factories.
class EntityFactory
{
public:
	typedef QMap<QString, QVariant> Properties;

	EntityFactory() {}
	virtual ~EntityFactory() {}

	//Returns the name of the class this factory is for.
	virtual QString className() const=0;

	//Creates the entity with the given properties and returns a Pointer to it.
	virtual Pointer<Entity> buildEntity(Properties properties, Pointer<Entity> parent) const=0;
	//Takes any action necessary to handle a child widget.
	virtual void addChild(QString type, Pointer<Entity> child) const=0;

	//Returns a set of every property name the factory accepts.
	virtual QSet<QString> getPropertyNames() const=0;

	//Sets a named property.
	virtual void setProperty(Pointer<Entity> e, QString name, QVariant value) const=0;
	//Returns the value of a named property.
	virtual QVariant getProperty(Pointer<Entity> e, QString name) const=0;
};

//A base class that does a lot of work and can be subclassed and partially redefined.
template<class E>
class SimpleEntityFactory : public EntityFactory
{
public:
	SimpleEntityFactory(): mClassName(typeid(E())) {}
	virtual ~SimpleEntityFactory() {}

	QString className() const { return mClassName; }

	virtual Pointer<Entity> buildEntity(Properties properties, Pointer<Entity> parent) const;
	virtual void addChild(QString type, Pointer<Entity> child) const {}

	virtual QSet<QString> getPropertyNames() const;

	void setProperty(Pointer<Entity> e, QString name, QVariant value) const;
	//Overloaded version with the template parameter passed to it instead.
	virtual void setProperty(Pointer<E> e, QString name, QVariant value) const {}

	QVariant getProperty(Pointer<Entity> e, QString name) const;
	//Overloaded version
	virtual QVariant getProperty(Pointer<E> e, QString name) const {}

protected:
	//Called by default buildEntity function. Only constructs entity and does not set properties apart from those needed for construction.
	virtual Pointer<Entity> construct(Properties properties, Pointer<Entity> parent) const=0;
	//Can be overloaded if special action is necessary to set the position.
	virtual void setPos(Pointer<E> e, Math::Point pos) const;

private:
	QString mClassName;
};

//Generic factory function
template<class E>
Pointer<Entity> SimpleEntityFactory<E>::buildEntity(Properties properties, Pointer<Entity> parent) const
{
	Pointer<Entity> entity = this->construct(properties, parent);

	QMapIterator i(properties);
	while (i.hasNext()) {
		i.next();
		setProperty(entity, i.key(), i.value());
	}
}

template<class E>
QSet<QString> SimpleEntityFactory<E>::getPropertyNames() const
{
	static QSet<QString> names;
	//initialize
	if (names.empty()) {
		names
			<< "name"
			<< "pos"
			<< "visible"
			<< "movable";
	}

	return names;
}

template<class E>
void SimpleEntityFactory<E>::setProperty(Pointer<Entity> e, QString name, QVariant value) const
{
	if (name == "name")
		e->setName(value.toString());
	else if (name == "pos")
		this->setPos(e, value.value<Math::Point>());
	else if (name == "visible")
		e->setVisible(value.toBool());
	else if (name == "movable")
		e->setMovable(value.toBool());
	else {
		assert(e->inherits(mClassName));
		this->setProperty((Pointer<E>)e, name, value);
	}
}

template<class E>
QVariant SimpleEntityFactory<E>::getProperty(Pointer<Entity> e, QString name) const
{
	if (name == "name") return QVariant(e->name());
	else if (name == "pos") return QVariant::fromValue<Math::Point>(e->pos());
	else if (name == "visible") return QVariant(e->visible());
	else if (name == "movable") return QVariant(e->movable());
	else {
		assert(e->inherits(mClassName));
		this->getProperty((Pointer<E>)e, name, value);
	}
}

template<class E>
void SimpleEntityFactory<E>::setPos(Pointer<E> e, Math::Point pos) const
{
	e->setPos(pos);
}

//Same as SimpleEntityFactory but with a default construct() function implemented for classes that have a standard constructor.
template<class E>
class AutoEntityFactory : public SimpleEntityFactory<E> {
public:
	AutoEntityFactory(QString classname): SimpleEntityFactory<E>(classname) {}
	virtual ~AutoEntityFactory() {}

protected:
	virtual Pointer<Entity> construct(Properties properties, Pointer<Entity> parent) const;
};

template<class E>
Pointer<Entity> AutoEntityFactory<E>::construct(Properties properties, Pointer<Entity> parent) const
{
	E* ent = new E(properties["pos"].value<Math::Point>(), parent);
	return ent->pointer();
}

#endif /* ENTITYFACTORY_H_ */
