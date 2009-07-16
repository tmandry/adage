set(adage_SRCS
   Game.cpp
   GameUI.cpp
   main.cpp
   GameBlueprint.cpp
   BlueprintWindow.cpp
   CommWindow.cpp
   ShellInterface.cpp
   ShellWindow.cpp
   ../math/Polygon.cpp
   ../math/Segment.cpp
   ../ui/Blueprint.cpp
)

set(adage_MOC_HDRS
	Game.h
	GameUI.h
	GameBlueprint.h
	BlueprintWindow.h
	CommWindow.h
	ShellInterface.h
	../ui/Blueprint.h
)

#moc these files
QT4_WRAP_CPP(adage_MOC_SRCS ${adage_MOC_HDRS})

add_executable(adage ${adage_SRCS} ${adage_MOC_SRCS})

#QT LIBRARIES here
set(QT_USE_OPENGL TRUE)
include(${QT_USE_FILE})
include_directories(${QT_QTOPENGL_INCLUDE_DIR})

target_link_libraries(adage
	adage_shared
	${QT_LIBRARIES}
	${QT_QTOPENGL_LIBRARIES}
)

