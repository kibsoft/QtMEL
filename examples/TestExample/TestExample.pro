#-------------------------------------------------
#
# Project created by QtCreator 2013-02-16T19:25:34
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = TestExample
TEMPLATE = app

#add QVACL library
INCLUDEPATH += ../../include
LIBS += -L../../lib -lqvacl1

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
