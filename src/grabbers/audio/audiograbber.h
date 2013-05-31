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

#ifndef AUDIOGRABBER_H
#define AUDIOGRABBER_H

#include "../../qtmel_global.h"
#include "../abstractgrabber.h"

#include <QStringList>

class QAudioInput;
class RtAudio;

typedef unsigned int RtAudioStreamStatus;

int handleData(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                double streamTime, RtAudioStreamStatus status, void *data);

class AudioFormat {
public:
    enum Format {
        SignedInt8 = 0x1,
        SignedInt16 = 0x2,
        SignedInt24 = 0x4,
        SignedInt32 = 0x8,
        Float32 = 0x10,
        Float64 = 0x20
    };

    AudioFormat();
    ~AudioFormat();

    void setSampleRate(int rate);
    int sampleRate() const;

    void setFormat(AudioFormat::Format format);
    AudioFormat::Format format() const;

    void setChannelCount(int count);
    int channelCount() const;

private:
    int m_sampleRate;
    AudioFormat::Format m_format;
    int m_channelCount;
};

class QTMELSHARED_EXPORT AudioGrabber : public AbstractGrabber
{
    Q_OBJECT

public:
    friend int handleData(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                          double streamTime, RtAudioStreamStatus status, void *data);

    explicit AudioGrabber(QObject *parent = 0);
    ~AudioGrabber();

    void setDeviceIndex(int index);
    int deviceIndex() const;

    void setFormat(const AudioFormat &format);
    AudioFormat format() const;

    int grabbedAudioDataSize() const;

    int elapsedMilliseconds() const;

    static QStringList availableDeviceNames();

public Q_SLOTS:
    bool start();
    void stop();
    void suspend();
    void resume();

Q_SIGNALS:
    void dataAvailable(const QByteArray &data);

private:
    void init();
    void cleanup();
    void onDataAvailable(const QByteArray &data);

    AudioFormat m_format;
    int m_deviceIndex;
    int m_grabbedAudioDataSize;
    RtAudio *m_rtAudio;
};

#endif // AUDIOGRABBER_H
