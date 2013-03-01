#ifndef ENCODER_H
#define ENCODER_H

#include "qvacl_global.h"
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

    enum VideoCodec {
        None = 0,
        MPEG1, /*!< MPEG-1 video */
        MPEG2, /*!< MPEG-2 video */
        H261 = 4, /*!< H.261 */
        H263, /*!< H.263 / H.263-1996, H.263+ / H.263-1998 / H.263 version 2 */
        RV10, /*!< RealVideo 1.0 */
        RV20, /*!< RealVideo 2.0 */
        MJPEG, /*!< Motion JPEG */
        LJPEG = 10, /*!< Lossless JPEG */
        JPEGLS = 12, /*!< JPEG-LS */
        MPEG4, /*!< MPEG-4 part 2 */
        RAW, /*!< Raw video */
        MSMPEG4V2 = 16, /*!< MPEG-4 part 2 Microsoft variant version 2 */
        MSMPEG4V3, /*!< MPEG-4 part 2 Microsoft variant version 3 */
        WMV1, /*!< Windows Media Video 7 */
        WMV2, /*!< Windows Media Video 8 */
        H263P, /*!< H.263+ / H.263-1998 / H.263 version 2 */
        FLV1 = 21, /*!< FLV / Sorenson Spark / Sorenson H.263 (Flash Video) */
        SVQ1, /*!< Sorenson Vector Quantizer 1 / Sorenson Video 1 / SVQ1 */
        DVVIDEO = 24, /*!< DV (Digital Video) */
        HUFFYUV, /*!< HuffYUV */
        H264 = 27, /*!< H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10 */
        THEORA = 30, /*!< Theora */
        ASV1, /*!< ASUS V1 */
        ASV2, /*!< ASUS V2 */
        FFV1, /*!< FFmpeg video codec #1 */
        CLJR = 36, /*!< Cirrus Logic AccuPak */
        ROQ = 38, /*!< RoQ video */
        MSVIDEO1 = 46, /*!< Microsoft Video 1 */
        ZLIB = 54, /*!< LCL (LossLess Codec Library) ZLIB */
        QTRLE, /*!< QuickTime Animation (RLE) video */
        SNOW, /*!< Snow */
        PNG = 62, /*!< PNG (Portable Network Graphics) image */
        PPM, /*!< PPM (Portable PixelMap) image */
        PBM, /*!< PBM (Portable BitMap) image */
        PGM, /*!< PGM (Portable GrayMap) image */
        PGMYUV, /*!< PGMYUV (Portable GrayMap YUV) image */
        PAM, /*!< PAM (Portable AnyMap) image */
        FFVHUFF, /*!< Huffyuv FFmpeg variant */
        BMP = 79, /*!< BMP (Windows and OS/2 bitmap) */
        ZMBV = 82, /*!< Zip Motion Blocks Video */
        FLASHSV = 87, /*!< Flash Screen Video v1 */
        CAVS, /*!< Chinese AVS (Audio Video Standard) (AVS1-P2, JiZhun profile) */
        JPEG2000, /*!< JPEG 2000 */
        TARGA = 94, /*!< Truevision Targa image */
        TIFF = 97, /*!< TIFF image */
        GIF, /*!< GIF (Graphics Interchange Format) */
        DNXHD = 101, /*!< VC3/DNxHD */
        SGI = 103, /*!< SGI image */
        AMV = 109, /*!< AMV Video */
        PCX = 111, /*!< PC Paintbrush PCX image */
        SUNRAST, /*!< Sun Rasterfile image */
        DIRAC = 120, /*!< Dirac */
        V210 = 131, /*!< Uncompressed 4:2:2 10-bit */
        DPX, /*!< DPX image */
        FLASHSV2 = 135, /*!< Flash Screen Video v2 */
        R210 = 137, /*!< Uncompressed RGB 10-bit */
        VP8 = 144, /*!< On2 VP8 */
        A64_MULTI = 147, /*!< Multicolor charset for Commodore 64 */
        A64_MULTI5, /*!< Multicolor charset for Commodore 64, extended with 5th color */
        R10K, /*!< AJA Kona 10-bit RGB Codec */
        PRORES = 152 /*!< Apple ProRes (iCodec Pro) */
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
