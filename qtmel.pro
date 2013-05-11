TEMPLATE = lib

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "Kirill Bukaev(aka KIBSOFT)"
QMAKE_TARGET_COPYRIGHT = "Copyright © 2013 Kirill Bukaev(aka KIBSOFT)"
QMAKE_TARGET_PRODUCT = "QtMEL"
QMAKE_TARGET_DESCRIPTION = "Qt Media Encoding Library"

DESTDIR = $$PWD/lib
OBJECTS_DIR = build/obj
MOC_DIR = build/moc

DEFINES += QTMEL_LIBRARY
QT += multimedia
CONFIG += debug_and_release build_all no_keywords

CONFIG(debug, debug|release) {
    TARGET = qtmeld
} else {
    TARGET = qtmel
}

win32 {
    LIBS += -L$$(FFMPEG_LIBRARY_PATH) -L$$(OPENCV_LIBRARY_PATH)
    INCLUDEPATH += $$(FFMPEG_INCLUDE_PATH) $$(OPENCV_INCLUDE_PATH)
    SOURCES += src/helpers/mousehelper_win.cpp
}

LIBS += -lavcodec-53 \
    -lavformat-53 \
    -lswscale-2 \
    -lavutil-51 \
    -llibopencv_highgui243

SOURCES += src/grabbers/abstractgrabber.cpp \
    src/grabbers/image/abstractimagegrabber.cpp \
    src/grabbers/image/screengrabber.cpp \
    src/helpers/mousehelper.cpp \
    src/encoder/encoder.cpp \
    src/encoder/videocodecsettings.cpp \
    src/grabbers/audio/audiograbber.cpp \
    src/encoder/audiocodecsettings.cpp \
    src/grabbers/image/cameragrabber.cpp

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
    src/encoder/audiocodecsettings.h \
    src/grabbers/image/cameragrabber.h
