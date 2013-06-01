#include "recorder.h"

#include "../encoder/encoder.h"
#include "../grabbers/image/abstractimagegrabber.h"
#include "../grabbers/image/screengrabber.h"
#include "../grabbers/image/cameragrabber.h"
#include "../grabbers/audio/audiograbber.h"
#include "../helpers/audiotimer.h"

#include <QTimer>
#include <QEventLoop>
#include <QDebug>

Recorder::Recorder(QObject *parent) :
    QObject(parent)
  , m_screenGrabber(0)
  , m_cameraGrabber(0)
  , m_audioGrabber(0)
  , m_encoder(new Encoder(this))
{
    connect(m_encoder, SIGNAL(started()), this, SLOT(startGrabbers()));
}

void Recorder::setImageGrabber(AbstractImageGrabber *grabber)
{
    if (dynamic_cast<ScreenGrabber *>(grabber)) {
        m_screenGrabber = dynamic_cast<ScreenGrabber *>(grabber);
        m_cameraGrabber = 0;
    } else if (dynamic_cast<CameraGrabber *>(grabber)) {
        m_cameraGrabber = dynamic_cast<CameraGrabber *>(grabber);
        m_screenGrabber = 0;
    }
}

AbstractImageGrabber *Recorder::imageGrabber() const
{
    return castImageGrabber();
}

void Recorder::setAudioGrabber(AudioGrabber *grabber)
{
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

void Recorder::start()
{
    if (!m_screenGrabber && !m_cameraGrabber && !m_audioGrabber) {
        setError(tr("There are no any grabbers."));
        return;
    }

    m_encoder->start();
}

void Recorder::pause()
{
    AbstractImageGrabber *grabber = castImageGrabber();

    if (grabber)
        grabber->suspend();

    if (m_audioGrabber)
        m_audioGrabber->suspend();
}

void Recorder::resume()
{
    AbstractImageGrabber *grabber = castImageGrabber();

    if (grabber)
        grabber->resume();

    if (m_audioGrabber)
        m_audioGrabber->resume();
}

void Recorder::stop()
{
    AbstractImageGrabber *grabber = castImageGrabber();

    if (grabber) {
        qDebug()<<"Grabbed frames:"<<grabber->grabbedFrameCount();
        grabber->stop();
    }

    if (m_audioGrabber) {
        qDebug()<<"Grabbed audio data:"<<m_audioGrabber->grabbedAudioDataSize();
        m_audioGrabber->stop();
    }

    qDebug()<<"Encoded frame count: "<<m_encoder->encodedFrameCount();
    qDebug()<<"Encoded audio data: "<<m_encoder->encodedAudioDataSize();

    m_encoder->stop();

}

void Recorder::startGrabbers()
{
    AbstractImageGrabber *grabber = castImageGrabber();

    if (grabber) {
        connect(grabber, SIGNAL(frameAvailable(QImage,int)), m_encoder, SLOT(encodeVideoFrame(QImage,int)));

        if (m_cameraGrabber)
            connect(m_cameraGrabber, SIGNAL(initialized()), m_audioGrabber, SLOT(start()));

        if (m_audioGrabber) {
            grabber->setTimer(new AudioTimer(m_audioGrabber, this));
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
