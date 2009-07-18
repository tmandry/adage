//Entities.h:
//
// Includes and registers all entities that have a factory.
//
#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "FactoryManager.h"

//INCLUDE ENTITIES HERE
#include "world/Building.h"
#include "world/GhostBustersHQ.h"
#include "world/GhostPortal.h"
#include "world/GhostTrap.h"
#include "world/Wall.h"
#include "actors/Ghost.h"
#include "actors/GhostBuster.h"
#include "actors/Navigator.h"
#include "actors/Person.h"

inline void registerEntities()
{
	FactoryManager* fm = FactoryManager::instance();

	//REGISTER ENTITIES HERE
	fm->registerEntity<Building>();
	fm->registerEntity<GhostBustersHQ>();
	fm->registerEntity<DormantGhostPortal>();
	fm->registerEntity<GhostPortal>();
	fm->registerEntity<GhostTrap>();
	fm->registerEntity<Wall>();
	fm->registerEntity<Ghost>();
	fm->registerEntity<GhostBuster>();
	fm->registerEntity<Navigator>();
	fm->registerEntity<Person>();
	//fm->registerEntity<>();
}

#endif /* ENTITIES_H_ */
