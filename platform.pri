!editor {
	CONFIG -= qt
} else {
	CONFIG += qt
}

win32 {
	DEFINES += __WINDOWS__ PLATFORM_WIN32 __WIN32__
	INCLUDEPATH += $${BASE}/contrib/ $${BASE}/contrib/windows/
	CONFIG -= glfw
	CONFIG -= sdl
	CONFIG += sdl2
	LIBS += -L$${BASE}/contrib/windows/ -lseed -lseedcontrib -mwindows -lmingw32 -lopengl32 -lopenal32 -lversion -limm32 -lole32 -loleaut32 -lwinmm -lws2_32

	sdl {
		DEFINES += WIN32 main=SDL_main
		LIBS += -lSDLmain -lSDL -lSDL_image -lgdi32
	}
}

macx {
	DEFINES += __MACOSX__
	CONFIG -= sdl
	CONFIG -= glfw
	CONFIG += sdl2
	INCLUDEPATH += contrib/osx/

	sdl2 {
		LIBS += -framework OpenAL -framework OpenGL -framework Cocoa -framework IOKit -framework QTKit -framework CoreFoundation -framework CoreAudio -framework AudioUnit -framework ForceFeedback -framework Carbon -framework AudioToolbox
		DEFINES += TARGET_API_MAC_OSX
	}
}

unix:!macx {
	DEFINES += __LINUX__ PHYSFS_NO_CDROM_SUPPORT
	CONFIG -= sdl
	CONFIG += sdl2
}

qt {
	DEFINES += BUILD_QT
	QT += opengl
} else:glfw {
	DEFINES += BUILD_GLFW
} else:sdl {
	DEFINES += BUILD_SDL
} else:sdl2 {
	DEFINES += _THREAD_SAFE BUILD_SDL2
}

CONFIG(debug, debug|release) {
	DEFINES += DEBUG
	LIBS += -L$${BASE}/lib/debug
} else {
	DEFINES += RELEASE
	LIBS += -L$${BASE}/lib/release
}
