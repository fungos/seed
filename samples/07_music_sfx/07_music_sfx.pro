TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += glfw

INCLUDEPATH += ../../include ../../contrib

SOURCES += main.cpp \
	sfx_sample.cpp

HEADERS += \
	sfx_sample.h

OTHER_FILES += \
	../bin/sfx_app.config \
	../bin/sfx_sample.scene \
	../bin/sfx_sample.config \
	../bin/sfx.ogg \
	../bin/theme.ogg

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