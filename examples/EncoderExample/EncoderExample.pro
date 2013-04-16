#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T03:01:52
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EncoderExample
TEMPLATE = app

#add QVACL library
INCLUDEPATH += ../../include
LIBS += -L../../lib -lqtmel1

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
