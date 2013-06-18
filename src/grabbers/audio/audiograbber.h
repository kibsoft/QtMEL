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

class QTMELSHARED_EXPORT AudioFormat {
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

//! The AudioGrabber class allows the application to capture data from audio input devices.
/*!
  Using this class you can capture audio data from devices available on your system. In order to get available devices
  call the availableDeviceNames() static function. To let AudioGrabber know which input device it must grab pass the device index to the setDeviceIndex() function.
  Also you can adjust the quality of audio by passing AudioFormat object to the setFormat() function. During the process of capture you can call the grabbedAudioDataSize()
  function that returns the size of grabbed audio data.

  The signal dataAvailable() is emmited whenever a new data chunk is available.

  Here is an example of AudioGrabber usage:
  @code
    AudioFormat format;
    format.setChannelCount(2);
    format.setSampleRate(44100);
    format.setFormat(AudioFormat::SignedInt16);

    AudioGrabber *audioGrabber = new AudioGrabber(this);
    audioGrabber->setDeviceIndex(0);
    audioGrabber->setFormat(format);
    audioGrabber->start();
  @endcode
*/
class QTMELSHARED_EXPORT AudioGrabber : public AbstractGrabber
{
    Q_OBJECT

    friend int handleData(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                          double streamTime, RtAudioStreamStatus status, void *data);

public:
    explicit AudioGrabber(QObject *parent = 0);
    ~AudioGrabber();

    /*!
      Lets AudioGrabber know which device it must grab.
      \sa deviceIndex()
      \sa availableDeviceNames()
    */
    void setDeviceIndex(int index);
    /*!
      Returns current device index.
      \sa setDeviceIndex()
    */
    int deviceIndex() const;

    /*!
      Sets the given format.
      \sa format()
    */
    void setFormat(const AudioFormat &format);
    /*!
      Returns current audio format.
      \sa setFormat()
    */
    AudioFormat format() const;

    /*!
      Returns the size of grabbed audio data.
      When the grabber is not active returns 0.
      \sa elapsedMilliseconds()
    */
    int grabbedAudioDataSize() const;
    /*!
      Returns time in milliseconds from start of grabbing process till this moment.
      When the grabber is not active returns 0.
      \sa grabbedAudioDataSize()
    */
    int elapsedMilliseconds() const;

    /*!
      Returns names of available cameras.
    */
    static QStringList availableDeviceNames();

public Q_SLOTS:
    bool start();
    void stop();
    void suspend();
    void resume();

Q_SIGNALS:
    /*!
      This signal is emmited whenever a new data chunk is available.
    */
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
