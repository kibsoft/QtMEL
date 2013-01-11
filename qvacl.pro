QT       -= gui

TARGET = qvacl
TEMPLATE = lib
VERSION = 1.0.0

DESTDIR = lib
OBJECTS_DIR = obj
MOC_DIR = moc

DEFINES += QVACL_LIBRARY

SOURCES += src/grabbers/abstractgrabber.cpp

HEADERS += src/grabbers/abstractgrabber.h\
        src/qvacl_global.h
