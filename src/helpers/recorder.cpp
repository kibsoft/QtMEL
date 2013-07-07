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
  , m_imageGrabber(0)
  , m_audioGrabber(0)
  , m_encoder(new Encoder(this))
  , m_state(Recorder::StoppedState)
  , m_startMuteTime(-1)
  , m_muted(false)
{
    qRegisterMetaType<AbstractGrabber::State>("Recorder::State");

    connect(m_encoder, SIGNAL(started()), this, SLOT(startGrabbers()));
}

void Recorder::setImageGrabber(AbstractImageGrabber *grabber)
{
    if (grabber->state() == AbstractImageGrabber::StoppedState) {
        //stop an old grabber if there is
        if (m_imageGrabber) {
            m_imageGrabber->stop();
            m_imageGrabber->setTimer(0);
            m_imageGrabber->disconnect(m_encoder);
        }

        m_imageGrabber = grabber;

        if (m_audioGrabber) {
            grabber->setTimer(m_timer);
        }

        if (state() == Recorder::ActiveState || state() == Recorder::SuspendedState) {
            connect(grabber, SIGNAL(frameAvailable(QImage,int)), m_encoder, SLOT(encodeVideoFrame(QImage,int)), Qt::UniqueConnection);

            grabber->start();

            if (state() == Recorder::SuspendedState)
                grabber->suspend();
        }
    }
}

AbstractImageGrabber *Recorder::imageGrabber() const
{
    return m_imageGrabber;
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

bool Recorder::isMuted() const
{
    return m_muted;
}

void Recorder::start()
{
    if (state() == Recorder::StoppedState) {
        if (!m_imageGrabber && !m_audioGrabber) {
            setError(tr("There are no any grabbers."));
            return;
        }

        m_encoder->start();
    }
}

void Recorder::pause()
{
    if (state() == Recorder::ActiveState) {
        if (m_imageGrabber)
            m_imageGrabber->suspend();

        if (m_audioGrabber)
            m_audioGrabber->suspend();

        setState(Recorder::SuspendedState);
    }
}

void Recorder::resume()
{
    if (state() == Recorder::SuspendedState) {
        if (m_imageGrabber)
            m_imageGrabber->resume();

        if (m_audioGrabber)
            m_audioGrabber->resume();

        setState(Recorder::ActiveState);
    }
}

void Recorder::stop()
{
    if (state() != Recorder::StoppedState) {
        if (m_imageGrabber) {
            m_imageGrabber->stop();
        }

        if (m_audioGrabber) {
            m_audioGrabber->stop();
            m_imageGrabber->setTimer(0);
            delete m_timer;
        }

        m_encoder->stop();

        m_startMuteTime = -1;

        setState(Recorder::StoppedState);
    }
}

void Recorder::mute()
{
    if (m_audioGrabber) {
        m_muted = true;
        disconnectAudioGrabber();
        m_startMuteTime = m_audioGrabber->elapsedMilliseconds();
    }
}

void Recorder::unmute()
{
    if (m_audioGrabber) {
        m_muted = false;
        if (m_startMuteTime != -1) {
            int muteTime = m_audioGrabber->elapsedMilliseconds() - m_startMuteTime;
            encodeSilence(muteTime);
        }

        connectAudioGrabber();
    }
}

void Recorder::startGrabbers()
{
    if (m_imageGrabber) {
        connect(m_imageGrabber, SIGNAL(frameAvailable(QImage,int)), m_encoder, SLOT(encodeVideoFrame(QImage,int)), Qt::UniqueConnection);

        connect(m_imageGrabber, SIGNAL(initialized()), m_audioGrabber, SLOT(start()));

        if (m_audioGrabber) {
            m_timer = new AudioTimer(m_audioGrabber, this);
            m_imageGrabber->setTimer(m_timer);
        }
    }

    //if audio mute is not turned on
    if (m_startMuteTime == -1 && m_audioGrabber)
        connectAudioGrabber();

    if (m_imageGrabber) {
        if (!m_imageGrabber->start()) {
            m_imageGrabber->disconnect(m_encoder);
            onGrabbersError();
            return;
        }
    }

    //if we record only audio
    if (m_audioGrabber && !m_imageGrabber) {
        if (!m_audioGrabber->start()) {
            disconnectAudioGrabber();
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

bool Recorder::connectAudioGrabber()
{
    return connect(m_audioGrabber, SIGNAL(dataAvailable(QByteArray)), m_encoder, SLOT(encodeAudioData(QByteArray)), Qt::UniqueConnection);
}

bool Recorder::disconnectAudioGrabber()
{
    return disconnect(m_audioGrabber, SIGNAL(dataAvailable(QByteArray)), m_encoder, SLOT(encodeAudioData(QByteArray)));
}

void Recorder::encodeSilence(int milliseconds)
{
    if (m_audioGrabber) {
        AudioFormat format = m_audioGrabber->format();

        int sampleSize = 0;
        switch (format.format()) {
        case AudioFormat::SignedInt8:
            sampleSize = 1;
            break;

        case AudioFormat::SignedInt16:
            sampleSize = 2;
            break;

        case AudioFormat::SignedInt24:
            sampleSize = 3;
            break;

        case AudioFormat::SignedInt32:
        case AudioFormat::Float32:
            sampleSize = 4;
            break;

        case AudioFormat::Float64:
            sampleSize = 8;
            break;
        }


        int silenceDataSize = ((format.sampleRate() * format.channelCount() * sampleSize) / 1000) * milliseconds;
        QByteArray silenceData;
        silenceData.fill('\0', silenceDataSize);
        m_encoder->encodeAudioData(silenceData);
    }
}
