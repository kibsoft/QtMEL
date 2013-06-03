#-------------------------------------------------
#
# Project created by QtCreator 2013-06-03T13:26:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CameraExample
TEMPLATE = app

#add QtMEL library
INCLUDEPATH += ../../include
LIBS += -L../../lib

CONFIG(debug, debug|release) {
    LIBS += -lqtmeld1
} else {
    LIBS += -lqtmel1
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
