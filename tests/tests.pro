TEMPLATE = app
CONFIG += console
CONFIG -= qt

INCLUDEPATH += ../include ../contrib
DEFINES += BUILD_SDL

SOURCES += main.cpp \
	testbase.cpp \
	jsonreader.cpp

HEADERS += \
	testbase.h \
	jsonreader.h

OTHER_FILES += \
	bin/body.sprite \
	bin/sample.movie \
	bin/logo.sprite \
	bin/head.sprite \
	bin/upperbody.sprite

CONFIG(debug, debug|release) {
	DESTDIR = $(SEEDSDK)/seed/tests/bin
	DEFINES += DEBUG
	LIBS += -L$(SEEDSDK)/seed/lib/debug -lseed -lseedcontrib
} else {
	DESTDIR = $(SEEDSDK)/seed/tests/bin
	DEFINES += RELEASE
	LIBS += -L$(SEEDSDK)/seed/lib/release -lseed -lseedcontrib
}

unix {
	DEFINES += LINUX
	LIBS += -lGL -lopenal -lSDL -lSDL_image
}

win32 {
	DEFINES += WIN32
	LIBS += -lopengl32 -lopenal32 -lSDL -lSDL_image
}
