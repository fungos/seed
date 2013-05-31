TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += glfw

#TARGET_EXT = .bc
#QMAKE_EXT_OBJ = .bc
#QMAKE_CXXFLAGS += -emit-llvm
#QMAKE_CXX = clang++
#QMAKE_CC = clang
#QMAKE_LIB = llvm-ld -link-as-library -o
#QMAKE_RUN_CXX = $(CXX) $(CXXFLAGS) $(INCPATH) -c $src -o $obj
#QMAKE_RUN_CC = $(CC) $(CCFLAGS) $(INCPATH) -c $src -o $obj

INCLUDEPATH += ../include ../contrib

SOURCES += main.cpp \
	testbase.cpp

HEADERS += \
	testbase.h

OTHER_FILES_CONFIG = \
#Configs
		resources/configs/app.config

OTHER_FILES_SCENE = \
#Scenes
		resources/scenes/main.scene

OTHER_FILES_TEXTURE = \
#Textures
		resources/textures/frame01.png \
		resources/textures/frame02.png \
		resources/textures/frame03.png \
		resources/textures/particle_00.tga \
		resources/textures/particle_01.tga \
		resources/textures/particle_02.tga \
		resources/textures/particle_03.tga \
		resources/textures/particle_04.tga \
		resources/textures/particle_05.tga \
		resources/textures/particle_06.tga \
		resources/textures/particle_07.tga \
		resources/textures/particle_08.tga \
		resources/textures/particle_09.tga \
		resources/textures/particle_10.tga \
		resources/textures/particle_11.tga \
		resources/textures/particle_12.tga \
		resources/textures/particle_13.tga \
		resources/textures/particle_14.tga \
		resources/textures/particle_15.tga

OTHER_FILES_SOUND = \
#Sounds
		resources/sounds/sound.sfx

OTHER_FILES_PARTICLE = \
#Particles
		resources/particles/particles.sprite

OTHER_FILES_MOVIE = \
#Movies
		resources/movies/sample.movie

OTHER_FILES += $${OTHER_FILES_CONFIG} \
		$${OTHER_FILES_SCENE} \
		$${OTHER_FILES_TEXTURE} \
		$${OTHER_FILES_SOUND} \
		$${OTHER_FILES_PARTICLE} \
		$${OTHER_FILES_MOVIE}

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

		#Configs
		APP_CONFIG_FILES.files = $$OTHER_FILES_CONFIG
		APP_CONFIG_FILES.path = Contents/Resources/configs
		#Scenes
		APP_SCENE_FILES.files = $$OTHER_FILES_SCENE
		APP_SCENE_FILES.path = Contents/Resources/scenes
		#Textures
		APP_TEXTURE_FILES.files = $$OTHER_FILES_TEXTURE
		APP_TEXTURE_FILES.path = Contents/Resources/textures
		#Sounds
		APP_SOUND_FILES.files = $$OTHER_FILES_SOUND
		APP_SOUND_FILES.path = Contents/Resources/sounds
		#Particles
		APP_PARTICLE_FILES.files = $$OTHER_FILES_PARTICLE
		APP_PARTICLE_FILES.path = Contents/Resources/particles
		#Movies
		APP_MOVIE_FILES.files = $$OTHER_FILES_MOVIE
		APP_MOVIE_FILES.path = Contents/Resources/movies

		QMAKE_BUNDLE_DATA += APP_CONFIG_FILES APP_SCENE_FILES APP_TEXTURE_FILES \
						APP_GUI_TEXTURE_FILES APP_SOUND_FILES APP_PARTICLE_FILES \
						APP_MOVIE_FILES
}

win32 {
	LIBS += -L../contrib/windows/ -lseed -lseedcontrib -mwindows -lmingw32 -lopengl32 -lopenal32
	INCLUDEPATH += ../contrib/windows/
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
