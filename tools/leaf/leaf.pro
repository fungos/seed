QT += widgets network

TARGET = leaf
TEMPLATE = app

FLAGSXX += -std=c++11

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

QMAKE_CXXFLAGS += $$FLAGSXX #-emit-llvm
