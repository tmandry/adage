project(adage)
#FILES
set(adage_shared_SRCS
   actors/Actor.cpp
   actors/Ghost.cpp
   actors/GhostBuster.cpp
   actors/Navigator.cpp
   actors/Person.cpp
   actors/steering/Alignment.cpp
   actors/steering/Arrive.cpp
   actors/steering/AvoidWalls.cpp
   actors/steering/Cohesion.cpp
   actors/steering/Evade.cpp
   actors/steering/Flee.cpp
   actors/steering/Pursue.cpp
   actors/steering/Seek.cpp
   actors/steering/Separation.cpp
   actors/steering/Wander.cpp
   map/Map.cpp
   map/MapGenerator.cpp
   map/MapHandler.cpp
   nav/AStar.cpp
   nav/NavNode.cpp
   nav/NavSystem.cpp
   world/Building.cpp
   world/CellSpacePartition.cpp
   world/Entity.cpp
   world/FactoryManager.cpp
   world/GameBase.cpp
   world/GhostBustersHQ.cpp
   world/GhostPortal.cpp
   world/GhostTrap.cpp
   world/MovingEntity.cpp
   world/View.cpp
   world/Wall.cpp
   world/World.cpp
)

add_library(adage_shared SHARED ${adage_shared_SRCS})

#BUILD OPTIONS
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif(CMAKE_COMPILER_IS_GNUCXX)

#QT LIBRARIES go here
set(QT_USE_XML TRUE)
include(${QT_USE_FILE})
include_directories(
	.
	${QT_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
)
target_link_libraries(adage_shared
	${QT_LIBRARIES}
	${QT_QTXML_LIBRARIES}
)

subdirs(adage editor)

