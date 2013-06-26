#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T03:01:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EncoderExample
TEMPLATE = app

#add QtMEL library
INCLUDEPATH += ../../include

greaterThan(QT_MAJOR_VERSION, 4) {
    LIBS += -L../../libsQt5
} else {
    LIBS += -L../../libsQt4
}


unix {
    DEFINES += QTMEL_NOT_INSTALLED
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
