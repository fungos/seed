TEMPLATE = app
CONFIG += console
CONFIG -= qt

INCLUDEPATH += ../include ../contrib

DEFINES += BUILD_SDL

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

unix {
	DEFINES += LINUX
	LIBS += -lseed -lseedcontrib -lGL -lopenal -lSDL -lSDL_image
}

win32 {
	DEFINES += WIN32 main=SDL_main
	LIBS += -L../contrib/windows/ -lseed -lseedcontrib -mwindows -lmingw32 -lSDLmain -lSDL -lopengl32 -lopenal32 -lSDL_image -lgdi32
	INCLUDEPATH += ../contrib/windows/
}
