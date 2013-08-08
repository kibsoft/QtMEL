TEMPLATE = lib

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "Kirill Bukaev(aka KIBSOFT)"
QMAKE_TARGET_COPYRIGHT = "Copyright © 2013 Kirill Bukaev(aka KIBSOFT)"
QMAKE_TARGET_PRODUCT = "QtMEL"
QMAKE_TARGET_DESCRIPTION = "Qt Media Encoding Library"

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
111 dsfdsfds s 1
#only for Qt 5.x
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets multimedia
    DESTDIR = $$PWD/libsQt5
} else {
    #use Qt Mobility for Qt 4.x
    CONFIG += mobility
    MOBILITY += multimedia

    DESTDIR = $$PWD/libsQt4
}

DEFINES += QTMEL_LIBRARY
CONFIG += debug_and_release build_all no_keywords


CONFIG(debug, debug|release) {
    TARGET = qtmeld
} else {
    TARGET = qtmel
}

INCLUDEPATH += $$(FFMPEG_INCLUDE_PATH) $$(OPENCV_INCLUDE_PATH) ./src/3rdparty/RtAudio/include
LIBS += -L$$(FFMPEG_LIBRARY_PATH) -L$$(OPENCV_LIBRARY_PATH)

win32 {
    DEFINES += __WINDOWS_DS__
    LIBS += -ldsound -lole32 -lwinmm
    SOURCES += src/helpers/mousehelper_win.cpp

    LIBS += -lavcodec-53 \
    -lavformat-53 \
    -lswscale-2 \
    -lavutil-51 \
    -ldsound \
    -lole32 \
    -lwinmm

    win32-g++ {
        LIBS += -llibopencv_highgui243
    } else {
        LIBS += -lopencv_highgui243
    }
}

unix {
    DEFINES += __LINUX_ALSA__
    SOURCES += src/helpers/mousehelper_x11.cpp

    LIBS += -lavcodec \
    -lavformat \
    -lswscale \
    -lavutil \
    -lopencv_highgui \
    -lasound

    #make install
    headers1.files = ./src/grabbers/image/*.h
    headers1.path = /usr/local/include/src/grabbers/image

    headers2.files = ./src/grabbers/audio/*.h
    headers2.path = /usr/local/include/src/grabbers/audio

    headers3.files = ./src/grabbers/*.h
    headers3.path = /usr/local/include/src/grabbers

    headers4.files = ./src/encoder/*.h
    headers4.path = /usr/local/include/src/encoder

    headers5.files = ./src/helpers/*.h
    headers5.path = /usr/local/include/src/helpers

    headers6.files = ./src/*.h
    headers6.path = /usr/local/include/src

    headers7.files = ./include/*
    headers7.path = /usr/local/include

    greaterThan(QT_MAJOR_VERSION, 4) {
        libs.files = ./libsQt5/*
    } else {
        libs.files = ./libsQt4/*
    }
    libs.path = /usr/local/lib

    INSTALLS += headers1 headers2 headers3 headers4 headers5 headers6 headers7 libs
}

SOURCES += src/grabbers/abstractgrabber.cpp \
    src/grabbers/image/abstractimagegrabber.cpp \
    src/grabbers/image/screengrabber.cpp \
    src/helpers/mousehelper.cpp \
    src/encoder/encoder.cpp \
    src/encoder/videocodecsettings.cpp \
    src/grabbers/audio/audiograbber.cpp \
    src/encoder/audiocodecsettings.cpp \
    src/grabbers/image/cameragrabber.cpp \
    src/3rdparty/RtAudio/RtAudio.cpp \
    src/helpers/recorder.cpp \
    src/helpers/audiotimer.cpp

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
    src/grabbers/image/cameragrabber.h \
    src/3rdparty/RtAudio/RtError.h \
    src/3rdparty/RtAudio/RtAudio.h \
    src/helpers/recorder.h \
    src/helpers/audiotimer.h
