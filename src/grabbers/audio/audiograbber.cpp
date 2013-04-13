#include "audiograbber.h"

#include <QAudioInput>

AudioGrabber::AudioGrabber(QObject *parent)
    : AbstractGrabber(parent)
{
    init();
}

AudioGrabber::~AudioGrabber()
{
    cleanup();
}


void AudioGrabber::setDevice(const QAudioDeviceInfo &device)
{
    m_deviceInfo = device;
}

QAudioDeviceInfo AudioGrabber::device() const
{
    return m_deviceInfo;
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

bool AudioGrabber::start()
{
    if (state() == AbstractGrabber::StoppedState) {
        if (m_deviceInfo.isNull()) {
            setError(AbstractGrabber::DeviceNotFoundError, tr("Device to be grabbed was not found."));
            return false;
        }

        if (!m_deviceInfo.isFormatSupported(m_format)) {
            setError(AbstractGrabber::InvalidFormatError, tr("The format is not supported by the device."));
            return false;
        }

        m_inputDevice = new QAudioInput(m_deviceInfo, m_format);
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

        emit frameAvailable(data);
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
