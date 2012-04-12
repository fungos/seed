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
	bin/anim.sprite \
	bin/sample.movie \
	bin/logo.sprite \
	bin/out.movie \
	bin/out.sprite

CONFIG(debug, debug|release) {
	DESTDIR = $$(SEEDSDK)/seed/tests/bin
	DEFINES += DEBUG
	LIBS += -L$$(SEEDSDK)/seed/lib/debug
} else {
	DESTDIR = $$(SEEDSDK)/seed/tests/bin
	DEFINES += RELEASE
	LIBS += -L$$(SEEDSDK)/seed/lib/release
}

unix {
	DEFINES += LINUX
	LIBS += -lseed -lseedcontrib -lGL -lopenal -lSDL -lSDL_image
}

win32 {
	DEFINES += WIN32 main=SDL_main
	LIBS += -lseed -lseedcontrib -mwindows -lmingw32 -lSDLmain -lSDL -lopengl32 -lopenal32 -lSDL_image -lgdi32
	INCLUDEPATH += ../contrib/windows/
}
