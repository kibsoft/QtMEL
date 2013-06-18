/****************************************************************************
**
** QtMEL is a Qt Media Encoding Library that allows to encode video and audio streams
** Copyright (C) 2013 Kirill Bukaev(aka KIBSOFT).
** Contact: Kirill Bukaev (support@kibsoft.ru)
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/

#ifndef RECORDER_H
#define RECORDER_H

#include "../qtmel_global.h"

#include <QObject>
#include <QImage>

class AbstractImageGrabber;
class ScreenGrabber;
class CameraGrabber;
class AudioGrabber;
class Encoder;
class AudioTimer;

//! The Recorder class helps you easily record audio/video data.
/*!
  Using this class you can easily create audio and video files with data grabbed from the passed devices. Before start() you must setup encoder()
  and set an image grabber and/or an audio grabber calling setImageGrabber() and setAudioGrabber() correspondingly.

  The recorder takes care of audio/video streams syncronization(if both grabbers are set).

  <B>Limitations: </B>The Recorder class only supports screen and camera image grabbers, it means you can't pass your own image grabber using setImageGrabber().
*/
class QTMELSHARED_EXPORT Recorder : public QObject
{
    Q_OBJECT
public:
    /*! This enum describes the state of the recorder. */
    enum State {
        ActiveState = 0,
        SuspendedState,
        StoppedState
    };

    explicit Recorder(QObject *parent = 0);
    
    void setImageGrabber(AbstractImageGrabber *grabber);
    AbstractImageGrabber* imageGrabber() const;

    void setAudioGrabber(AudioGrabber *grabber);
    AudioGrabber* audioGrabber() const;

    Encoder* encoder() const;

    Recorder::State state() const;

public Q_SLOTS:
    /*! Starts audio/video recodring. The state() is set to Recorder::ActiveState if no errors occurred. */
    void start();
     /*! Pauses audio/video recodring. The state() is set to Recorder::SuspendedState. */
    void pause();
    /*! Resumes audio/video recodring. The state() is set to Recorder::ActiveState. */
    void resume();
    /*! Stops audio/video recodring. The state() is set to Recorder::StoppedState. */
    void stop();

    /*! Mutes audio volume (if audioGrabber() is set) */
    void mute();
    /*! Unmutes audio volume (if audioGrabber() is set) */
    void unmute();

Q_SIGNALS:
    /*! This signal is emitted when the recorder state has changed. */
    void stateChanged(Recorder::State state);
    /*! This signal is emitted when an error occurs. */
    void error(const QString &errorString);

private Q_SLOTS:
    void startGrabbers();

private:
    void setState(Recorder::State state);
    void setError(const QString &errorString);
    void onGrabbersError();
    AbstractImageGrabber* castImageGrabber() const;

    bool connectAudioGrabber();
    bool disconnectAudioGrabber();

    void encodeSilence(int milliseconds);

    ScreenGrabber *m_screenGrabber;
    CameraGrabber *m_cameraGrabber;
    AudioGrabber *m_audioGrabber;
    Encoder *m_encoder;
    AudioTimer *m_timer;
    Recorder::State m_state;
    int m_startMuteTime;
};

#endif // RECORDER_H
