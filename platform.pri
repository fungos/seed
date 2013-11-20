!editor {
	CONFIG -= qt
} else {
	CONFIG += qt
}

win32 {
	INCLUDEPATH += contrib/windows/
	CONFIG -= glfw
	CONFIG += sdl
}

macx {
	!editor:!glfw {
		message("Seed for OSX must use GLFW, disabling SDL.")
		CONFIG -= sdl
		CONFIG += glfw
	}
	INCLUDEPATH += contrib/osx/
}

unix {
	DEFINES += LINUX
	CONFIG += sdl
}

qt {
	DEFINES += BUILD_QT
	QT += opengl
} else:glfw {
	DEFINES += BUILD_GLFW
} else:sdl {
	DEFINES += BUILD_SDL
}

CONFIG(debug, debug|release) {
	DEFINES += DEBUG
} else {
	DEFINES += RELEASE
}
