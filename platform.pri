!editor {
	CONFIG -= qt
} else {
	CONFIG += qt
}

win32 {
	INCLUDEPATH += contrib/windows/
	CONFIG -= glfw
	CONFIG -= sdl
	CONFIG += sdl2
}

macx {
	DEFINES += __MACOSX__
	CONFIG -= sdl
	CONFIG -= glfw
	CONFIG += sdl2
	INCLUDEPATH += contrib/osx/

	sdl2 {
		LIBS += -framework OpenAL -framework OpenGL -framework Cocoa -framework IOKit -framework QTKit -framework CoreFoundation -framework CoreAudio -framework AudioUnit -framework ForceFeedback -framework Carbon -framework AudioToolbox
		DEFINES += TARGET_API_MAC_OSX _THREAD_SAFE
	}
}

unix:!macx {
	DEFINES += __LINUX__ PHYSFS_NO_CDROM_SUPPORT
	CONFIG -= sdl
	CONFIG += sdl2
	sdl2 {
		DEFINES += _THREAD_SAFE
	}
}

qt {
	DEFINES += BUILD_QT
	QT += opengl
} else:glfw {
	DEFINES += BUILD_GLFW
} else:sdl {
	DEFINES += BUILD_SDL
} else:sdl2 {
	DEFINES += BUILD_SDL2
}

CONFIG(debug, debug|release) {
	DEFINES += DEBUG
} else {
	DEFINES += RELEASE
}
