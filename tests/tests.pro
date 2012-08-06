TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += glfw

INCLUDEPATH += ../include ../contrib

SOURCES += main.cpp \
	testbase.cpp

HEADERS += \
	testbase.h

OTHER_FILES += \
	bin/anim.sprite \
	bin/sample.movie \
	bin/logo.sprite \
	bin/out.movie \
	bin/out.sprite \
	bin/particles.sprite \
	bin/teste.emitter \
	bin/app.config

CONFIG(debug, debug|release) {
	DESTDIR = ../tests/bin
	DEFINES += DEBUG
	LIBS += -L../lib/debug
} else {
	DESTDIR = ../tests/bin
	DEFINES += RELEASE
	LIBS += -L../lib/release
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
	INCLUDEPATH += ../contrib/osx/
	LIBS += -lseed -lseedcontrib -framework OpenAL -framework OpenGL -framework Cocoa -framework IOKit
	CONFIG -= sdl
	CONFIG += glfw
}

win32 {
	LIBS += -L../contrib/windows/ -lseed -lseedcontrib -mwindows -lmingw32 -lopengl32 -lopenal32
	INCLUDEPATH += ../contrib/windows/
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
