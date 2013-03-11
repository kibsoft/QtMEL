#ifndef ENCODER_H
#define ENCODER_H

#include "../qvacl_global.h"
#include <QObject>
#include <QSize>

class EncoderObject;
class QThread;

class QVACLSHARED_EXPORT Encoder : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QSize videoSize READ videoSize WRITE setVideoSize NOTIFY videoSizeChanged)
    Q_PROPERTY(int fixedFrameRate READ fixedFrameRate WRITE setFixedFrameRate NOTIFY fixedFrameRateChanged)
    Q_PROPERTY(bool encodeAudio READ encodeAudio WRITE setEncodeAudio NOTIFY encodeAudioChanged)

public:

    enum Error {
        NoError = 0, /*!< No error occurred. */
        InvalidOutputFormatError, /*!< Could not get output format by filename. */
        InvalidFormatContextError, /*!< Could not allocate format context. */
        InvalidVideoStreamError, /*!< Could not allocate video stream. */
        VideoEncoderNotFoundError, /*!< Requested video encoder was not found. */
        InvalidVideoCodecError, /*!< Could not open video codec. */
        InvalidAudioStreamError, /*!< Could not allocate audio stream. */
        AudioEncoderNotFoundError, /*!< Requested audio encoder was not found. */
        InvalidAudioCodecError, /*!< Could not open audio codec. */
        FileOpenError /*!< Could not open a file. */
    };

    explicit Encoder(QObject *parent = 0);
    ~Encoder();

    void setFilePath(const QString &filePath);
    QString filePath() const;

    void setVideoSize(const QSize &size);
    QSize videoSize() const;

    void setFixedFrameRate(int frameRate);
    int fixedFrameRate() const;

    void setEncodeAudio(bool encode);
    bool encodeAudio() const;

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

signals:
    void filePathChanged(const QString &filePath);
    void videoSizeChanged(const QSize &size);
    void fixedFrameRateChanged(int frameRate);
    void encodeAudioChanged(bool encode);

    void error(Encoder::Error errorCode);

private:
    void setError(Encoder::Error errorCode, const QString &errorString);

    EncoderObject *m_encoder;
    QThread *m_encoderThread;

    Encoder::Error m_error;
    QString m_errorString;
};

#endif // ENCODER_H
