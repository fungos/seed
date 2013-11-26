!editor {
	CONFIG -= qt
} else {
	CONFIG += qt
}

win32 {
	INCLUDEPATH += contrib/windows/
	CONFIG += glfw
	CONFIG -= sdl
	CONFIG -= sdl2
}

macx {
	CONFIG -= sdl
	CONFIG += glfw
	CONFIG -= sdl2
	INCLUDEPATH += contrib/osx/
}

unix {
	DEFINES += LINUX
	CONFIG -= sdl
	CONFIG -= sdl2
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
