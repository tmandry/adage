#include "FactoryManager.h"

FactoryManager::FactoryManager()
{
}

FactoryManager::~FactoryManager()
{
}

FactoryManager* FactoryManager::instance()
{
	static FactoryManager* fm = new FactoryManager;

	return fm;
}

EntityFactory* FactoryManager::getFactory(QString type) const
{
	return mFactories.value(type, 0);
}
