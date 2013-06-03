#include "recorder.h"

#include "../encoder/encoder.h"
#include "../grabbers/image/abstractimagegrabber.h"
#include "../grabbers/image/screengrabber.h"
#include "../grabbers/image/cameragrabber.h"
#include "../grabbers/audio/audiograbber.h"
#include "../helpers/audiotimer.h"

#include <QTimer>
#include <QEventLoop>
#include <QMetaType>

Recorder::Recorder(QObject *parent) :
    QObject(parent)
  , m_screenGrabber(0)
  , m_cameraGrabber(0)
  , m_audioGrabber(0)
  , m_encoder(new Encoder(this))
  , m_state(Recorder::StoppedState)
{
    qRegisterMetaType<AbstractGrabber::State>("Recorder::State");

    connect(m_encoder, SIGNAL(started()), this, SLOT(startGrabbers()));
}

void Recorder::setImageGrabber(AbstractImageGrabber *grabber)
{
    if (state() == Recorder::StoppedState) {
        if (dynamic_cast<ScreenGrabber *>(grabber)) {
            m_screenGrabber = dynamic_cast<ScreenGrabber *>(grabber);
            m_cameraGrabber = 0;
        } else if (dynamic_cast<CameraGrabber *>(grabber)) {
            m_cameraGrabber = dynamic_cast<CameraGrabber *>(grabber);
            m_screenGrabber = 0;
        }
    }
}

AbstractImageGrabber *Recorder::imageGrabber() const
{
    return castImageGrabber();
}

void Recorder::setAudioGrabber(AudioGrabber *grabber)
{
    if (state() == Recorder::StoppedState)
        m_audioGrabber = grabber;
}

AudioGrabber *Recorder::audioGrabber() const
{
    return m_audioGrabber;
}

Encoder *Recorder::encoder() const
{
    return m_encoder;
}

Recorder::State Recorder::state() const
{
    return m_state;
}

void Recorder::start()
{
    if (state() == Recorder::StoppedState) {
        if (!m_screenGrabber && !m_cameraGrabber && !m_audioGrabber) {
            setError(tr("There are no any grabbers."));
            return;
        }

        m_encoder->start();
    }
}

void Recorder::pause()
{
    if (state() == Recorder::ActiveState) {
        AbstractImageGrabber *grabber = castImageGrabber();

        if (grabber)
            grabber->suspend();

        if (m_audioGrabber)
            m_audioGrabber->suspend();

        setState(Recorder::SuspendedState);
    }
}

void Recorder::resume()
{
    if (state() == Recorder::SuspendedState) {
        AbstractImageGrabber *grabber = castImageGrabber();

        if (grabber)
            grabber->resume();

        if (m_audioGrabber)
            m_audioGrabber->resume();

        setState(Recorder::ActiveState);
    }
}

void Recorder::stop()
{
    if (state() != Recorder::StoppedState) {
        AbstractImageGrabber *grabber = castImageGrabber();

        if (grabber) {
            grabber->stop();
        }

        if (m_audioGrabber) {
            m_audioGrabber->stop();
            grabber->setTimer(0);
            delete m_timer;
        }

        m_encoder->stop();

        setState(Recorder::StoppedState);
    }
}

void Recorder::startGrabbers()
{
    AbstractImageGrabber *grabber = castImageGrabber();

    if (grabber) {
        connect(grabber, SIGNAL(frameAvailable(QImage,int)), m_encoder, SLOT(encodeVideoFrame(QImage,int)));

        if (m_cameraGrabber)
            connect(m_cameraGrabber, SIGNAL(initialized()), m_audioGrabber, SLOT(start()));

        if (m_audioGrabber) {
            m_timer = new AudioTimer(m_audioGrabber, this);
            grabber->setTimer(m_timer);
        }
    }

    if (m_audioGrabber) {
        connect(m_audioGrabber, SIGNAL(dataAvailable(QByteArray)), m_encoder, SLOT(encodeAudioData(QByteArray)));
    }

    if (grabber) {
        if (!grabber->start()) {
            grabber->disconnect();
            onGrabbersError();
            return;
        }
    }

    if (m_audioGrabber && (m_screenGrabber || (!m_screenGrabber && !m_cameraGrabber))) {
        if (!m_audioGrabber->start()) {
            m_audioGrabber->disconnect();
            onGrabbersError();
            return;
        }
    }

    setState(Recorder::ActiveState);
}

void Recorder::setState(Recorder::State state)
{
    if (m_state != state) {
        m_state = state;

        Q_EMIT stateChanged(state);
    }
}

void Recorder::setError(const QString &errorString)
{
    Q_EMIT error(errorString);
}

void Recorder::onGrabbersError()
{
    m_encoder->stop();

    setError(tr("Unable to start grabbers."));
}

AbstractImageGrabber *Recorder::castImageGrabber() const
{
    return m_screenGrabber ? dynamic_cast<AbstractImageGrabber *>(m_screenGrabber) : dynamic_cast<AbstractImageGrabber *>(m_cameraGrabber);
}
