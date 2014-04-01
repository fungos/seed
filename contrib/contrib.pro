QT -= qt

TARGET = seedcontrib
TEMPLATE = lib

QMAKE_CXXFLAGS -= -fno-rtti

CONFIG += staticlib sdl2 rocket box2d yajl physfs oggvorbistheora freetype vectormath

BASE = ..
#include(../compiler.pri)
include(../platform.pri)

FLAGSXX -= -stdlib=libc++

CONFIG(debug, debug|release) {
	DESTDIR = ../lib/debug
	DEFINES += DEBUG
} else {
	DESTDIR = ../lib/release
	DEFINES += RELEASE
}

SOURCES += \
	glew/glew.c \
	soil/stb_image_aug.c \
	soil/SOIL.c \
	soil/image_helper.c \
	soil/image_DXT.c \

HEADERS += \
	glew/wglew.h \
	glew/glxew.h \
	glew/glew.h \
	soil/stbi_DDS_aug.h \
	soil/stbi_DDS_aug_c.h \
	soil/stb_image_aug.h \
	soil/SOIL.h \
	soil/image_helper.h \
	soil/image_DXT.h \

OTHER_FILES += \
	README.md \
	flash/Console.as \

box2d:include(box2d.pri)
bullet:include(bullet.pri)
glfw:include(glfw.pri)
sdl2:include(sdl2.pri)
yajl:include(yajl.pri)
rocket:include(rocket.pri)
otterui:include(otterui.pri)
physfs:include(physfs.pri)
freetype:include(freetype.pri)
lua:include(lua.pri)
oggvorbistheora:include(oggvorbistheora.pri)
vectormath:include(vectormath.pri)
