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
        MPEG1 = 1, /*!< MPEG-1 video */
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

    enum AudioCodec {
        PCM_S16LE = 65536, /*!< PCM signed 16-bit little-endian */
        PCM_S16BE, /*!< PCM signed 16-bit big-endian */
        PCM_U16LE, /*!< PCM unsigned 16-bit little-endian */
        PCM_U16BE, /*!< PCM unsigned 16-bit big-endian */
        PCM_S8, /*!< PCM signed 8-bit */
        PCM_U8, /*!< PCM unsigned 8-bit */
        PCM_MULAW, /*!< PCM mu-law / G.711 mu-law */
        PCM_ALAW, /*!< PCM A-law / G.711 A-law */
        PCM_S32LE, /*!< PCM signed 32-bit little-endian */
        PCM_S32BE, /*!< PCM signed 32-bit big-endian */
        PCM_U32LE, /*!< PCM unsigned 32-bit little-endian */
        PCM_U32BE, /*!< PCM unsigned 32-bit big-endian */
        PCM_S24LE, /*!< PCM signed 24-bit little-endian */
        PCM_S24BE, /*!< PCM signed 24-bit big-endian */
        PCM_U24LE, /*!< PCM unsigned 24-bit little-endian */
        PCM_U24BE, /*!< PCM unsigned 24-bit big-endian */
        PCM_S24DAUD, /*!< PCM D-Cinema audio signed 24-bit */
        PCM_S16LE_PLANAR = 65554, /*!< PCM signed 16-bit little-endian planar */
        PCM_F32BE = 65556, /*!< PCM 32-bit floating point big-endian */
        PCM_F32LE, /*!< PCM 32-bit floating point little-endian */
        PCM_F64BE, /*!< PCM 64-bit floating point big-endian */
        PCM_F64LE, /*!< PCM 64-bit floating point little-endian */
        ADPCM_IMA_QT = 69632, /*!< ADPCM IMA QuickTime */
        ADPCM_IMA_WAV, /*!< ADPCM IMA WAV */
        ADPCM_MS = 69638, /*!< ADPCM Microsoft */
        ADPCM_ADX = 69641, /*!< SEGA CRI ADX ADPCM */
        ADPCM_G726 = 69643, /*!< G.726 ADPCM */
        ADPCM_SWF = 69645, /*!< ADPCM Shockwave Flash */
        ADPCM_YAMAHA, /*!< ADPCM Yamaha */
        ADPCM_G722 = 69660, /*!< G.722 ADPCM */
        AMR_NB = 73728, /*!< AMR-NB (Adaptive Multi-Rate NarrowBand) */
        AMR_WB, /*!< AMR-WB (Adaptive Multi-Rate WideBand) */
        RA_144 = 77824, /*!< RealAudio 1.0 (14.4K) */
        ROQ_DPCM = 81920, /*!< DPCM id RoQ */
        MP2 = 86016, /*!< MP2 (MPEG audio layer 2) */
        MP3, /*!< MP3 (MPEG audio layer 3) */
        AAC, /*!< AAC (Advanced Audio Coding) */
        AC3, /*!< ATSC A/52A (AC-3) */
        DTS, /*!< DCA (DTS Coherent Acoustics) */
        VORBIS, /*!< Vorbis */
        WMAV1 = 86023, /*!< Windows Media Audio 1 */
        WMAV2, /*!< Windows Media Audio 2 */
        SONIC = 86028, /*!< Sonic */
        SONIC_LS, /*!< Sonic lossless */
        FLAC, /*!< FLAC (Free Lossless Audio Codec) */
        ALAC = 86034, /*!< ALAC (Apple Lossless Audio Codec) */
        GSM = 86036, /*!< Gsm */
        GSM_MS = 86048, /*!< GSM Microsoft variant */
        NELLYMOSER = 86052, /*!< Nellymoser Asao */
        SPEEX = 86054, /*!< Speex */
        EAC3 = 86059 /*!< ATSC A/52B (AC-3, E-AC-3) */
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
