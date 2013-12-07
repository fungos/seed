TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += sdl2

INCLUDEPATH += ../../include ../../contrib
include("../compiler.pri")

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
		LIBS += -lseed -lseedcontrib -lGL -lopenal -lX11 -lpthread -lrt
		QMAKE_CXXFLAGS += -std=c++0x #-v

		sdl {
				LIBS += -lSDL -lSDL_image
		}
}

macx {
		DEFINES += LINUX TARGET_API_MAC_CARBON TARGET_API_MAC_OSX _THREAD_SAFE USE_API_SOIL
		INCLUDEPATH += ../../contrib/osx/
		LIBS += -lseed -lseedcontrib -framework OpenAL -framework OpenGL -framework Cocoa -framework IOKit -framework QTKit -framework CoreFoundation -framework CoreAudio -framework AudioUnit -framework ForceFeedback -framework Carbon -framework AudioToolbox
		CONFIG -= sdl
		CONFIG -= glfw
		CONFIG += sdl2

		APP_QML_FILES.files = $$OTHER_FILES
		APP_QML_FILES.path = Contents/Resources
		QMAKE_BUNDLE_DATA += APP_QML_FILES

		#sdl2 {
		#		LIBS += -lSDL -lSDL_image
		#}
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
} else:sdl2 {
		DEFINES += BUILD_SDL2
}

#LIBS += -L../../../libRocket/Build -lRocketCore -lRocketControls -lRocketDebugger
