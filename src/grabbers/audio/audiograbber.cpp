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
#include "../../3rdparty/RtAudio.h"

int handleData(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
               double streamTime, RtAudioStreamStatus status, void *data)
{
    Q_UNUSED(outputBuffer)
    Q_UNUSED(streamTime)
    Q_UNUSED(status)

    AudioGrabber *grabber = static_cast<AudioGrabber *>(data);

    int bytesReady = nBufferFrames * grabber->format().channelCount() * 2;
    QByteArray array;
    array.resize(bytesReady);
    memcpy(array.data(), inputBuffer, bytesReady);

    grabber->onDataAvailable(array);

    return 0;
}

AudioFormat::AudioFormat()
    : m_sampleRate(-1)
    , m_format(AudioFormat::SignedInt16)
    , m_channelCount(-1)
{
}

AudioFormat::~AudioFormat()
{
}

void AudioFormat::setSampleRate(int rate)
{
    m_sampleRate = rate;
}

int AudioFormat::sampleRate() const
{
    return m_sampleRate;
}

void AudioFormat::setFormat(AudioFormat::Format format)
{
    m_format = format;
}

AudioFormat::Format AudioFormat::format() const
{
    return m_format;
}

void AudioFormat::setChannelCount(int count)
{
    m_channelCount = count;
}

int AudioFormat::channelCount() const
{
    return m_channelCount;
}

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

void AudioGrabber::setFormat(const AudioFormat &format)
{
    m_format = format;
}

AudioFormat AudioGrabber::format() const
{
    return m_format;
}

int AudioGrabber::grabbedAudioDataSize() const
{
    return m_grabbedAudioDataSize;
}

int AudioGrabber::elapsedMilliseconds() const
{ 
    if (!m_rtAudio || state() == AudioGrabber::StoppedState)
        return 0;

    return m_rtAudio->getStreamTime() * 1000;
}

QHash<int, QString> AudioGrabber::availableDevices()
{
    RtAudio rtAudio;
    int deviceCount = rtAudio.getDeviceCount();
    RtAudio::DeviceInfo info;
    QHash<int, QString> devices;

    for (int i = 0; i < deviceCount; ++i) {
        info = rtAudio.getDeviceInfo(i);

        if (info.inputChannels > 0) {
            devices.insert(i, QString::fromStdString(info.name));
        }
    }

    return devices;
}

bool AudioGrabber::start()
{
    if (state() == AbstractGrabber::StoppedState) {
        if (deviceIndex() < 0 || deviceIndex() > availableDevices().count()) {
            setError(AbstractGrabber::DeviceNotFoundError, tr("Device to be grabbed was not found."));
            return false;
        }

        m_rtAudio = new RtAudio;

        unsigned int bufferFrames = 2048;
        RtAudio::StreamParameters params;
        params.deviceId = deviceIndex();
        params.nChannels = format().channelCount();
        params.firstChannel = 0;

        try {
            m_rtAudio->openStream(NULL, &params, format().format(), format().sampleRate(), &bufferFrames, &handleData, this);
            m_rtAudio->startStream();
        } catch (RtError&) {
            setError(AbstractGrabber::DeviceOpenError, tr("Unable to open device."));
            return false;
        }

        setState(AbstractGrabber::ActiveState);

        return true;
    }

    return false;
}

void AudioGrabber::stop()
{
    if (state() != AbstractGrabber::StoppedState) {
        m_rtAudio->closeStream();
        cleanup();

        setState(AbstractGrabber::StoppedState);
    }
}

void AudioGrabber::suspend()
{
    if (state() == AbstractGrabber::ActiveState) {
        m_rtAudio->stopStream();

        setState(AbstractGrabber::SuspendedState);
    }
}

void AudioGrabber::resume()
{
    if (state() == AbstractGrabber::SuspendedState) {
        m_rtAudio->startStream();

        setState(AbstractGrabber::ActiveState);
    }
}

void AudioGrabber::init()
{
    m_grabbedAudioDataSize = 0;
    m_rtAudio = 0;
}

void AudioGrabber::cleanup()
{
    if (m_rtAudio)
        delete m_rtAudio;

    init();
}

void AudioGrabber::onDataAvailable(const QByteArray &data)
{
    if (data.size() > 0) {
        m_grabbedAudioDataSize += data.size();

        Q_EMIT dataAvailable(data);
    }
}
