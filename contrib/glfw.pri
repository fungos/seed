SOURCES += \
	glfw/window.c \
	glfw/time.c \
#	glfw/thread.c \
	glfw/tga.c \
	glfw/stream.c \
	glfw/joystick.c \
	glfw/input.c \
	glfw/init.c \
	glfw/image.c \
	glfw/glext.c \
	glfw/fullscreen.c \
	glfw/enable.c

HEADERS += \
	glfw/internal.h \
	glfw/glfw.h \

unix:!macx:SOURCES += \
	glfw/x11/x11_enable.c \
	glfw/x11/x11_fullscreen.c \
	glfw/x11/x11_glext.c \
	glfw/x11/x11_init.c \
	glfw/x11/x11_joystick.c \
	glfw/x11/x11_keysym2unicode.c \
#	glfw/x11/x11_thread.c \
	glfw/x11/x11_time.c \
	glfw/x11/x11_window.c

macx:OBJECTIVE_SOURCES += \
	glfw/cocoa/cocoa_enable.m \
	glfw/cocoa/cocoa_fullscreen.m \
	glfw/cocoa/cocoa_glext.m \
	glfw/cocoa/cocoa_init.m \
	glfw/cocoa/cocoa_joystick.m \
#	glfw/cocoa/cocoa_thread.c \
	glfw/cocoa/cocoa_time.m \
	glfw/cocoa/cocoa_window.m

win32:SOURCES += \
	glfw/win32/win32_enable.c \
	glfw/win32/win32_fullscreen.c \
	glfw/win32/win32_glext.c \
	glfw/win32/win32_init.c \
	glfw/win32/win32_joystick.c \
#	glfw/win32/win32_thread.c \
	glfw/win32/win32_time.c \
	glfw/win32/win32_window.c

unix:!macx:HEADERS += glfw/x11/platform.h

macx:HEADERS += glfw/cocoa/platform.h

win32:HEADERS += glfw/win32/platform.h
