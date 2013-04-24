#-------------------------------------------------
#
# Project created by QtCreator 2013-02-16T19:25:34
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = TestExample
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
