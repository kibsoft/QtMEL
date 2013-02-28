TARGET = qvacl
TEMPLATE = lib
VERSION = 1.0.0

DESTDIR = $$PWD/lib
OBJECTS_DIR = obj
MOC_DIR = moc

DEFINES += QVACL_LIBRARY

win32 {
    LIBS += -L$$(FFMPEG_LIBRARY_PATH)
    INCLUDEPATH += $$(FFMPEG_INCLUDE_PATH)
}

LIBS += -lavcodec-53 \
    -lavformat-53

SOURCES += src/grabbers/abstractgrabber.cpp \
    src/grabbers/image/abstractimagegrabber.cpp \
    src/grabbers/image/screengrabber.cpp \
    src/helpers/mousehelper.cpp \
    src/encoder.cpp

win32: SOURCES += src/helpers/mousehelper_win.cpp

HEADERS += src/grabbers/abstractgrabber.h\
        src/qvacl_global.h \
    src/grabbers/image/abstractimagegrabber.h \
    src/grabbers/image/screengrabber.h \
    src/helpers/mousehelper.h \
    src/helpers/mousehelper_p.h \
    src/encoder.h
