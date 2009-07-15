project(adage)
#FILES
set(adage_SRCS
   adage/Game.cpp
   adage/GameUI.cpp
   adage/main.cpp
   adage/GameBlueprint.cpp
   adage/BlueprintWindow.cpp
   adage/CommWindow.cpp
   adage/ShellInterface.cpp
   adage/ShellWindow.cpp
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
   math/Polygon.cpp
   math/Segment.cpp
   nav/AStar.cpp
   nav/NavNode.cpp
   nav/NavSystem.cpp
   ui/Blueprint.cpp
   world/Building.cpp
   world/CellSpacePartition.cpp
   world/Entity.cpp
   world/GameBase.cpp
   world/GhostBustersHQ.cpp
   world/GhostPortal.cpp
   world/GhostTrap.cpp
   world/MovingEntity.cpp
   world/View.cpp
   world/Wall.cpp
   world/World.cpp
)

set(adage_MOC_HDRS
	adage/Game.h
	adage/GameUI.h
	adage/GameBlueprint.h
	adage/BlueprintWindow.h
	adage/CommWindow.h
	adage/ShellInterface.h
	ui/Blueprint.h
)

#moc these files
QT4_WRAP_CPP(adage_MOC_SRCS ${adage_MOC_HDRS})


add_executable(adage ${adage_SRCS} ${adage_MOC_SRCS})

#BUILD OPTIONS
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0")
endif(CMAKE_COMPILER_IS_GNUCXX)

#QT LIBRARIES go here
set(QT_USE_OPENGL TRUE)
set(QT_USE_XML TRUE)
include(${QT_USE_FILE})
include_directories(
	.
	${QT_INCLUDE_DIR}
	${QT_QTOPENGL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
)
target_link_libraries(adage
	${QT_LIBRARIES}
	${QT_QTOPENGL_LIBRARIES}
	${QT_QTXML_LIBRARIES}
)

