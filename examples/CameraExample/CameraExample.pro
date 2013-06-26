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

greaterThan(QT_MAJOR_VERSION, 4) {
    LIBS += -L../../libsQt5
} else {
    LIBS += -L../../libsQt4
}

unix {
    DEFINES += QTMEL_NOT_INSTALLED
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
