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

#ifndef ENCODER_H
#define ENCODER_H

#include "../qtmel_global.h"
#include "encoderglobal.h"
#include "videocodecsettings.h"
#include "audiocodecsettings.h"
#include <QObject>
#include <QSize>
#include <QImage>

class EncoderPrivate;
class QThread;

//! The Encoder class represents a media encoder.
/*!
  The class is used to encode video or audio streams.

  Here is an example of the Encoder usage:
  @code
  Encoder *encoder = new Encoder(this);
  encoder->setFileName("C:/QtEL/video.avi");
  encoder->setVideoSize(QSize(1280, 800));
  encoder->setFixedFrameRate(15);//set 15fps
  encoder->setOutputPixelFormat(EncoderGlobal::YUV420P);
  encoder->setVideoCodec(EncoderGlobal::H264);
  encoder->start();
  @endcode
*/
class QTMELSHARED_EXPORT Encoder : public QObject
{
    Q_OBJECT

    friend class EncoderPrivate;

public:

     /*! This enum describes encoder's errors. */
    enum Error {
        NoError = 0, /*!< No error occurred. */
        InvalidVideoSizeError, /*!< Wrong video size. */
        InvalidFilePathError, /*!< Empty file path. */
        InvalidOutputFormatError, /*!< Could not get output format by filename. */
        InvalidVideoStreamError, /*!< Could not add video stream. */
        VideoEncoderNotFoundError, /*!< Requested video encoder was not found. */
        InvalidVideoCodecError, /*!< Could not open video codec. */
        InvalidAudioStreamError, /*!< Could not add audio stream. */
        AudioEncoderNotFoundError, /*!< Requested audio encoder was not found. */
        InvalidAudioCodecError, /*!< Could not open audio codec. */
        FileOpenError, /*!< Could not open a file. */
        InvalidConversionContext, /*!< Could not initialize conversion context. */
        InvalidInputPixelFormat /*!< Could not convert input pixel format to the ffmpeg's format. */
    };

    /*! This enum describes the state of the encoder. */
    enum State {
        ActiveState = 0, /*!< Encoder is active, this state is set after start() is called. */
        StoppedState /*!< Encoder is not active. */
    };

    /*! This enum describes encoding modes. */
    enum EncodingMode  {
        VideoMode = 0, /*!< Encode only video stream. */
        AudioMode, /*!< Encode only audio stream. */
        VideoAudioMode /*!< Encode video and audio streams. */
    };

    /*! Constructs an encoder with the given parent. */
    explicit Encoder(QObject *parent = 0);
    /*! Destroys the encoder. */
    ~Encoder();

    /*!
      Sets the output file path.
      \sa filePath()
    */
    void setFilePath(const QString &filePath);
    /*!
      Returns output file path.
      \sa setFilePath()
    */
    QString filePath() const;

    void setVideoSize(const QSize &size);
    QSize videoSize() const;

    void setFixedFrameRate(int frameRate);
    int fixedFrameRate() const;

    void setEncodingMode(Encoder::EncodingMode mode);
    Encoder::EncodingMode encodingMode() const;

    void setOutputPixelFormat(EncoderGlobal::PixelFormat format);
    EncoderGlobal::PixelFormat outputPixelFormat() const;

    void setVideoCodec(EncoderGlobal::VideoCodec codec);
    EncoderGlobal::VideoCodec videoCodec() const;

    void setAudioCodec(EncoderGlobal::AudioCodec codec);
    EncoderGlobal::AudioCodec audioCodec() const;

    void setVideoCodecSettings(const VideoCodecSettings &settings);
    VideoCodecSettings videoCodecSettings() const;

    void setAudioCodecSettings(const AudioCodecSettings &settings);
    AudioCodecSettings audioCodecSettings() const;

    /*!
      Returns count of encoded video frames.
    */
    int encodedFrameCount() const;
    /*!
      Returns size of encoded audio data.
    */
    int encodedAudioDataSize() const;

    /*!
      Returns the state of the encoder.
      \sa stateChanged()
    */
    Encoder::State state() const;

    /*!
      Returns the last error that occurred. If no error occurred, returns Encoder::NoError.
      \sa errorString()
    */
    Encoder::Error error() const;

    /*!
      Returns a human-readable description of the last error that occurred.
      \sa error()
    */
    QString errorString() const;

public Q_SLOTS:
    /*! Starts encoding thread. The state() is set to Encoder::ActiveState if no errors occurred. */
    void start();
    /*! Stops encoding thread. The state() is set to Encoder::StoppedState. */
    void stop();

    /*!
      Encodes video frame. If encoding thread is in Encoder::StoppedState nothing happens.
      \param frame an image is to be encoded.
      \param duration time of frame display.
    */
    void encodeVideoFrame(const QImage &frame, int duration = -1);
    /*!
      Encodes audio data from passed byte array. If encoding thread is in Encoder::StoppedState nothing happens.
    */
    void encodeAudioData(const QByteArray &data);

Q_SIGNALS:
    /*! This signal is emitted when the encoder state has changed. */
    void stateChanged(Encoder::State state);
    /*! This signal is emitted when an error occurs. */
    void error(Encoder::Error errorCode);

private:
    void setState(Encoder::State state);

    void setError(Encoder::Error errorCode, const QString &errorString);

    EncoderPrivate *d_ptr;
    QThread *m_encoderThread;

    Encoder::State m_state;
    Encoder::Error m_error;
    QString m_errorString;
};

#endif // ENCODER_H
