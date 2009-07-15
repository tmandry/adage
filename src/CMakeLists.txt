project(adage)
cmake_minimum_required(VERSION 2.4.0)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

find_package(Qt4 REQUIRED)

#FILES
set(adage_SRCS
   Game.cpp
   GameUI.cpp
   main.cpp
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
   ui/BlueprintWindow.cpp
   ui/CommWindow.cpp
   ui/ShellInterface.cpp
   ui/ShellWindow.cpp
   world/Building.cpp
   world/CellSpacePartition.cpp
   world/Entity.cpp
   world/GhostBustersHQ.cpp
   world/GhostPortal.cpp
   world/GhostTrap.cpp
   world/MovingEntity.cpp
   world/View.cpp
   world/Wall.cpp
   world/World.cpp
)

set(adage_MOC_HDRS
	Game.h
	GameUI.h
	ui/Blueprint.h
	ui/BlueprintWindow.h
	ui/CommWindow.h
	ui/ShellInterface.h
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

