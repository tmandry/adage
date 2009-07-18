#ifndef FACTORYMANAGER_H_
#define FACTORYMANAGER_H_

#include <QHash>
#include <QString>
#include "Entity.h"
#include "EntityFactory.h"

class FactoryManager
{
public:
	static FactoryManager* instance();

	template<class E>
	void registerEntity();

	QList<QString> types() const { return mFactories.keys(); }
	EntityFactory* getFactory(QString type) const;

private:
	FactoryManager();
	~FactoryManager();

	QHash<QString, EntityFactory*> mFactories;
};

template<class E>
void FactoryManager::registerEntity()
{
	assert(!mFactories.contains(E::MetaInfo::className()));
	mFactories[E::MetaInfo::className()] = new typename E::Factory();
}

#endif /* FACTORYMANAGER_H_ */
