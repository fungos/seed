TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += glfw

INCLUDEPATH += ../../include ../../contrib

SOURCES += main.cpp \
	particle_sample.cpp

HEADERS += \
	particle_sample.h

OTHER_FILES += \
	../bin/particle_app.config \
	../bin/particle_00.tga \
	../bin/particle_01.tga \
	../bin/particle_02.tga \
	../bin/particle_03.tga \
	../bin/particle_04.tga \
	../bin/particle_05.tga \
	../bin/particle_06.tga \
	../bin/particle_07.tga \
	../bin/particle_08.tga \
	../bin/particle_09.tga \
	../bin/particle_10.tga \
	../bin/particle_11.tga \
	../bin/particle_12.tga \
	../bin/particle_13.tga \
	../bin/particle_14.tga \
	../bin/particle_15.tga \
	../bin/particle.sprite \
	../bin/particle_sample.scene \
	../bin/particle_sample.config

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

	APP_QML_FILES.files = $$OTHER_FILES
	APP_QML_FILES.path = Contents/Resources
	QMAKE_BUNDLE_DATA += APP_QML_FILES
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
