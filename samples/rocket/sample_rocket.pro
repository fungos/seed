TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += glfw

INCLUDEPATH += ../../include ../../contrib

SOURCES += main.cpp \
	rocket_sample.cpp

HEADERS += \
<<<<<<< HEAD
	image_sample.h
=======
	image_sample.h \
    rocket_sample.h
>>>>>>> a9580b74bf80685c754dd1f7b36da15e51452b9b

OTHER_FILES += ../bin/rocket_app.config \
	../bin/frame03.png \
	../bin/rocket_sample.scene \
	../bin/rocket_invader.rcss \
	../bin/rocket_invader.png \
	../bin/rocket_menu.rml \
	../bin/rocket_window.rml \
	../bin/Delicious-Roman.otf \
	../bin/Delicious-Italic.otf \
	../bin/Delicious-Bold.otf \
	../bin/Delicious-BoldItalic.otf

CONFIG(debug, debug|release) {
	DESTDIR = ../bin
	DEFINES += DEBUG
	LIBS += -L../../lib/debug
} else {
	DESTDIR = ../bin
	DEFINES += RELEASE
	LIBS += -L../../lib/release
}

unix:!macx {
	DEFINES += LINUX
	LIBS += -lseed -lseedcontrib -lGL -lopenal
	QMAKE_CXXFLAGS += -std=c++0x

	sdl {
		LIBS += -lSDL -lSDL_image
	}
}

macx {
	DEFINES += LINUX
	INCLUDEPATH += ../../contrib/osx/
	LIBS += -lseed -lseedcontrib -framework OpenAL -framework OpenGL -framework Cocoa -framework IOKit
	CONFIG -= sdl
	CONFIG += glfw
<<<<<<< HEAD
=======

	APP_QML_FILES.files = $$OTHER_FILES
	APP_QML_FILES.path = Contents/Resources
	QMAKE_BUNDLE_DATA += APP_QML_FILES
>>>>>>> a9580b74bf80685c754dd1f7b36da15e51452b9b
}

win32 {
	LIBS += -L../../contrib/windows/ -lseed -lseedcontrib -mwindows -lmingw32 -lopengl32 -lopenal32
	INCLUDEPATH += ../../contrib/windows/
	CONFIG -= glfw
	CONFIG += sdl
	sdl {
		DEFINES += WIN32 main=SDL_main
		LIBS += -lSDLmain -lSDL -lSDL_image -lgdi32
	}
}

glfw {
	DEFINES += BUILD_GLFW
} else:sdl {
	DEFINES += BUILD_SDL
}
<<<<<<< HEAD
=======

#LIBS += -L../../../libRocket/Build -lRocketCore -lRocketControls -lRocketDebugger
>>>>>>> a9580b74bf80685c754dd1f7b36da15e51452b9b
