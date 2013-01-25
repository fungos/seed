TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += glfw

INCLUDEPATH += ../../include ../../contrib

SOURCES += source/main.cpp \
	source/gameflow.cpp \
    source/states/credits_state.cpp \
    source/states/main_menu_state.cpp \
    source/states/options_state.cpp \
    source/states/game_pause_state.cpp \
    source/states/game_state.cpp \
    source/scenes/game_scene.cpp \
    source/data/game_data.cpp

HEADERS += \
	source/gameflow.h \
    source/states/main_menu_state.h \
    source/states/credits_state.h \
    source/states/options_state.h \
    source/states/game_pause_state.h \
    source/states/game_state.h \
    source/scenes/game_scene.h \
    source/data/game_data.h

OTHER_FILES += \
#Configs
	resources/configs/app.config \
	resources/configs/game.config \
#Fonts
	resources/fonts/Delicious-Roman.otf \
	resources/fonts/Delicious-Italic.otf \
	resources/fonts/Delicious-Bold.otf \
	resources/fonts/Delicious-BoldItalic.otf \
#Scenes
	resources/scenes/empty.scene \
	resources/scenes/game.scene \
#Textures
	resources/textures/rocket_invader.png \
	resources/textures/frame03.png \
#UIs
    #Styles
	resources/uis/styles/common.rcss \
	resources/uis/styles/window.rcss \
    #Views
	resources/uis/views/mainmenu.rml \
	resources/uis/views/window.rml \
	resources/uis/views/gameplay.rml \
	resources/uis/views/credits.rml \
	resources/uis/views/options.rml \
	resources/uis/views/game.rml \
	resources/uis/views/gamepause.rml

CONFIG(debug, debug|release) {
	DESTDIR = bin
	DEFINES += DEBUG
	LIBS += -L../../lib/debug
} else {
	DESTDIR = bin
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




















