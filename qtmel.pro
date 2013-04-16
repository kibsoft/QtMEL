TARGET = qtmel
TEMPLATE = lib
VERSION = 1.0.0

DESTDIR = $$PWD/lib
OBJECTS_DIR = obj
MOC_DIR = moc

DEFINES += QTMEL_LIBRARY

QT += multimedia
CONFIG += no_keywords

win32 {
    LIBS += -L$$(FFMPEG_LIBRARY_PATH)
    INCLUDEPATH += $$(FFMPEG_INCLUDE_PATH)
}

LIBS += -lavcodec-53 \
    -lavformat-53 \
    -lswscale-2 \
    -lavutil-51

SOURCES += src/grabbers/abstractgrabber.cpp \
    src/grabbers/image/abstractimagegrabber.cpp \
    src/grabbers/image/screengrabber.cpp \
    src/helpers/mousehelper.cpp \
    src/encoder/encoder.cpp \
    src/encoder/videocodecsettings.cpp \
    src/grabbers/audio/audiograbber.cpp \
    src/encoder/audiocodecsettings.cpp

win32: SOURCES += src/helpers/mousehelper_win.cpp

HEADERS += src/grabbers/abstractgrabber.h\
        src/qtmel_global.h \
    src/grabbers/image/abstractimagegrabber.h \
    src/grabbers/image/screengrabber.h \
    src/helpers/mousehelper.h \
    src/helpers/mousehelper_p.h \
    src/encoder/encoder.h \
    src/encoder/encoderglobal.h \
    src/encoder/videocodecsettings.h \
    src/grabbers/audio/audiograbber.h \
    src/encoder/audiocodecsettings.h
