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

#include "audiograbber.h"

#include <QAudioInput>

AudioGrabber::AudioGrabber(QObject *parent)
    : AbstractGrabber(parent)
    , m_deviceIndex(-1)
{
    init();
}

AudioGrabber::~AudioGrabber()
{
    cleanup();
}


void AudioGrabber::setDeviceIndex(int index)
{
    if (m_deviceIndex != index) {
        m_deviceIndex = index;
    }
}

int AudioGrabber::deviceIndex() const
{
    return m_deviceIndex;
}

void AudioGrabber::setFormat(const QAudioFormat &format)
{
    m_format = format;
}

QAudioFormat AudioGrabber::format() const
{
    return m_format;
}

int AudioGrabber::grabbedAudioDataSize() const
{
    return m_grabbedAudioDataSize;
}

QStringList AudioGrabber::availableDeviceNames()
{
    QAudioDeviceInfo device;
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QStringList names;

    Q_FOREACH (device, devices) {
        names.append(device.deviceName());
    }

    return names;
}

bool AudioGrabber::start()
{
    if (state() == AbstractGrabber::StoppedState) {
        if (m_deviceIndex < 0 || m_deviceIndex > availableDeviceNames().count()) {
            setError(AbstractGrabber::DeviceNotFoundError, tr("Device to be grabbed was not found."));
            return false;
        }

        QAudioDeviceInfo device = QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(m_deviceIndex);

        if (!device.isFormatSupported(m_format)) {
            setError(AbstractGrabber::InvalidFormatError, tr("The format is not supported by the device."));
            return false;
        }

        m_inputDevice = new QAudioInput(device, m_format);
        m_buffer = m_inputDevice->start();
        if (m_inputDevice->error() == QAudio::OpenError) {
            setError(AbstractGrabber::DeviceOpenError, tr("Unable to open device."));
            return false;
        }

        connect(m_buffer, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

        setState(AbstractGrabber::ActiveState);

        return true;
    }

    return false;
}

void AudioGrabber::stop()
{
    if (state() != AbstractGrabber::StoppedState) {
        m_inputDevice->stop();
        cleanup();

        setState(AbstractGrabber::StoppedState);
    }
}

void AudioGrabber::suspend()
{
    if (state() == AbstractGrabber::ActiveState) {
        m_inputDevice->suspend();

        setState(AbstractGrabber::SuspendedState);
    }
}

void AudioGrabber::resume()
{
    if (state() == AbstractGrabber::SuspendedState) {
        m_inputDevice->resume();

        setState(AbstractGrabber::ActiveState);
    }
}


void AudioGrabber::onReadyRead()
{
    if (m_buffer) {
        QByteArray data = m_buffer->readAll();
        m_grabbedAudioDataSize += data.size();

        Q_EMIT frameAvailable(data);
    }
}

void AudioGrabber::init()
{
    m_buffer = 0;
    m_grabbedAudioDataSize = 0;
}

void AudioGrabber::cleanup()
{
    if (m_inputDevice)
        delete m_inputDevice;

    init();
}
