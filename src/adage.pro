######################################################################
# Automatically generated by qmake (2.01a) Mon Dec 31 01:04:30 2007
######################################################################

TEMPLATE = app
TARGET = adage
CONFIG += debug
QT += opengl
DEPENDPATH += . math ui world
INCLUDEPATH += . world math ui
QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_LFLAGS_DEBUG += -O0
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

# Input
HEADERS += Game.h \
           GameUI.h \
           Pointer.h \
           actors/Actor.h \
           actors/Ghost.h \
           actors/GhostBuster.h \
           actors/Person.h \
           actors/steering/Alignment.h \
           actors/steering/Arrive.h \
           actors/steering/AvoidWalls.h \
           actors/steering/Cohesion.h \
           actors/steering/Evade.h \
           actors/steering/Flee.h \
           actors/steering/Pursue.h \
           actors/steering/Seek.h \
           actors/steering/Separation.h \
           actors/steering/SteeringBehavior.h \
           actors/steering/Wander.h \
           detail/StaticCastIterator.h \
           math/const.h \
           math/Point.h \
           math/real.h \
           math/Segment.h \
           math/Vector.h \
           ui/Blueprint.h \
           ui/BlueprintWindow.h \
           ui/CommWindow.h \
           ui/ShellInterface.h \
           ui/ShellWindow.h \
           world/Building.h \
           world/CellSpacePartition.h \
           world/Entity.h \
           world/EntityList.h \
           world/GhostBustersHQ.h \
           world/GhostTrap.h \
           world/Map.h \
           world/MovingEntity.h \
           world/View.h \
           world/Wall.h \
           world/World.h \
           world/trigger/LimitedLifetimeTrigger.h \
           world/trigger/Trigger.h \
           world/trigger/TriggerRegion.h
SOURCES += Game.cpp \
           GameUI.cpp \
           main.cpp \
           actors/Actor.cpp \
           actors/Ghost.cpp \
           actors/GhostBuster.cpp \
           actors/Person.cpp \
           actors/steering/Alignment.cpp \
           actors/steering/Arrive.cpp \
           actors/steering/AvoidWalls.cpp \
           actors/steering/Cohesion.cpp \
           actors/steering/Evade.cpp \
           actors/steering/Flee.cpp \
           actors/steering/Pursue.cpp \
           actors/steering/Seek.cpp \
           actors/steering/Separation.cpp \
           actors/steering/Wander.cpp \
           math/Segment.cpp \
           ui/Blueprint.cpp \
           ui/BlueprintWindow.cpp \
           ui/CommWindow.cpp \
           ui/ShellInterface.cpp \
           ui/ShellWindow.cpp \
           world/Building.cpp \
           world/CellSpacePartition.cpp \
           world/Entity.cpp \
           world/GhostBustersHQ.cpp \
           world/GhostTrap.cpp \
           world/Map.cpp \
           world/MovingEntity.cpp \
           world/Wall.cpp \
           world/World.cpp
