#ifndef ENCODER_H
#define ENCODER_H

#include "../qvacl_global.h"
#include <QObject>
#include <QSize>

class EncoderObject;
class QThread;

struct VideoCodecSettings {
    int bitrate; /*!< The average bitrate. */
    int gop; /*!< The number of pictures in a group of pictures. */
    int qmin; /*!< Minimum quantizer. */
    int qmax; /*!< Maximum  quantizer. */
    int maxQdiff; /*!< Maximum quantizer difference between frames. */
    int coderType; /*!< Coder type. */
    int meCmp; /*!< Motion estimation comparison function. */
    int partitions; /*!< Macroblock subpartition sizes to consider - p8x8, p4x4, b8x8, i8x8, i4x4. */
    int meMethod; /*!< Motion estimation algorithm used for video coding. */
    int meSubpelQuality; /*!< Subpel ME quality. */
    int meRange; /*!< Maximum motion estimation search range in subpel units If 0 then no limit. */
    int keyintMin; /*!< Minimum GOP size. */
    int scenechangeThreshold; /*!< Scene change detection threshold 0 is default, larger means fewer detected scene changes. */
    int iQuantFactor ; /*!< qscale factor between P and I-frames. */
    int bFrameStrategy  ; /*!<  */
    int qcompress; /*!< Amount of qscale change between easy & hard scenes (0.0-1.0). */
    int maxBFrames; /*!< Maximum number of B-frames between non-B-frames. */
    int refs; /*!< Number of reference frames. */
    int directpred; /*!< Direct MV prediction mode. */
    int trellis; /*!< Trellis RD quantization. */
    int weightedPPred; /*!< Explicit P-frame weighted prediction analysis method. */
    int rcLookahead; /*!< RC lookahead Number of frames for frametype and ratecontrol lookahead. */
    int flags; /*!< */
    int flags2; /*!< */

    int lumaElimThreshold; /*!< Luma single coefficient elimination threshold. */
    int chromaElimThreshold; /*!< Chroma single coeff elimination threshold. */
    int bQuantOffset; /*!< qscale offset between IP and B-frames. */
    int mpegQuant; /*!< 0-> h263 quant 1-> mpeg quant. */
    int rcEq; /*!< Rate control equation. */
    int rcMaxRate; /*!< Maximum bitrate. */
    int rcMinRate; /*!< Minimum bitrate. */
    int rcBufferSize; /*!< Decoder bitstream buffer size. */
};

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
