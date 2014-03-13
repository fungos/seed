TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += sdl2

BASE = ../../
INCLUDEPATH += $${BASE}/include
include("../platform.pri")
include("../compiler.pri")

DESTDIR = ../bin

unix:!macx {
		DEFINES += LINUX USE_API_SOIL SEED_ENABLE_OGL20
		LIBS += -lseed -lseedcontrib -lGL -lopenal -lX11 -lm -ldl -lpthread -lrt #-lc++abi
		QMAKE_CXXFLAGS += -std=c++11 #-v
}

macx {
		DEFINES += LINUX TARGET_API_MAC_CARBON TARGET_API_MAC_OSX _THREAD_SAFE USE_API_SOIL SEED_ENABLE_OGL20
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
