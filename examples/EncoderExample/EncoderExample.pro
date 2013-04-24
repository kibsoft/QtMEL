#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T03:01:52
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = EncoderExample
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
