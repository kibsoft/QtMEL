#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T03:01:52
#
#-------------------------------------------------

QT       += core gui

TARGET = EncoderExample
TEMPLATE = app

#add QtMEL library
INCLUDEPATH += ../../include
LIBS += -L../../lib


unix {
    LIBS += -lpthread -lX11
    QMAKE_CXXFLAGS += -std=c++0x
}

CONFIG(debug, debug|release) {
    win32: LIBS += -lqtmeld1
    unix: LIBS += -lqtmeld
} else {
    win32: LIBS += -lqtmel1
    unix: LIBS += -lqtmel
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
