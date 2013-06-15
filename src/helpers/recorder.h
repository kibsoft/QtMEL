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
    void start();
    void pause();
    void resume();
    void stop();

    void mute();
    void unmute();

Q_SIGNALS:
    void stateChanged(Recorder::State state);
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
