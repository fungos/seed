QT += widgets network

TARGET = leaf
TEMPLATE = app

SOURCES += \
		main.cpp\
		mainwindow.cpp \
		messagelistener.cpp \
		memorysortfiltermodel.cpp \
		filterwidget.cpp \
		memorywidget.cpp \
		memorymodel.cpp

HEADERS += \
		mainwindow.h \
		messagelistener.h \
		memorysortfiltermodel.h \
		filterwidget.h \
		memorywidget.h \
		memorymodel.h

RESOURCES += leaf.qrc

FORMS += mainwindow.ui
