set(editor_SRCS
	EditorGame.cpp
	EditorUI.cpp
	main.cpp
   ../math/Polygon.cpp
   ../math/Segment.cpp
   ../ui/Blueprint.cpp
)

set(editor_MOC_HDRS
	EditorUI.h
	../ui/Blueprint.h
)

set(editor_UIS
	editor.ui
)

#moc headers and generate UIs
QT4_WRAP_CPP(editor_MOC_SRCS ${editor_MOC_HDRS})
QT4_WRAP_UI(editor_UI_HDRS ${editor_UIS})

add_executable(editor ${editor_SRCS} ${editor_MOC_SRCS} ${editor_UI_HDRS})

#QT LIBRARIES here
set(QT_USE_OPENGL TRUE)
include(${QT_USE_FILE})
include_directories(${QT_QTOPENGL_INCLUDE_DIR})

target_link_libraries(editor
	adage_shared
	${QT_LIBRARIES}
	${QT_QTOPENGL_LIBRARIES}
)

