#include "encoder.h"

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

//ffmpeg include files
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include <QThread>

typedef uint8_t AVBuffer;

class EncoderPrivate : public QObject {
    Q_OBJECT

public:
    explicit EncoderPrivate(Encoder *e, QObject *parent = 0);
    ~EncoderPrivate();

    void setFilePath(const QString &filePath);
    QString filePath() const;

    void setVideoSize(const QSize &size);
    QSize videoSize() const;

    void setFixedFrameRate(int frameRate);
    int fixedFrameRate() const;

    void setEncodeAudio(bool encode);
    bool encodeAudio() const;

    void setVideoCodec(EncoderGlobal::VideoCodec codec);
    EncoderGlobal::VideoCodec videoCodec() const;

    void setAudioCodec(EncoderGlobal::AudioCodec codec);
    EncoderGlobal::AudioCodec audioCodec() const;

    void setVideoCodecSettings(const VideoCodecSettings &settings);
    VideoCodecSettings videoCodecSettings() const;

public slots:
    void start();

private:
    void init();

    bool createVideoStream();
    bool createAudioStream();

    bool openVideoStream();
    bool openAudioStream();

    void applyVideoCodecSettings();
    template <class T> void setVideoCodecOption(T AVCodecContext::*option, T (VideoCodecSettings::*f)() const);

    Encoder *q_ptr;

    EncoderGlobal::VideoCodec m_videoCodecName;
    EncoderGlobal::AudioCodec m_audioCodecName;
    VideoCodecSettings m_videoSettings;

    QString m_filePath;
    QSize m_videoSize;
    int m_fixedFrameRate;
    bool m_encodeAudio;

    //video stuff
    AVOutputFormat *m_outputFormat;
    AVFormatContext *m_formatContext;
    AVStream *m_videoStream;
    AVCodecContext *m_videoCodecContext;
    AVCodec *m_videoCodec;
    AVBuffer* m_videoBuffer;
    AVFrame *m_videoPicture;
    SwsContext *m_imageConvertContext;
    int m_videoBufferSize;

    //audio stuff
    AVStream *m_audioStream;
    AVCodecContext *m_audioCodecContext;
    AVCodec *m_audioCodec;
    AVBuffer *m_audioOutputBuffer;
    QByteArray m_audioInputBuffer;
    int m_audioOutputBufferSize;
    int m_audioSampleSize;
};

EncoderPrivate::EncoderPrivate(Encoder *e, QObject *parent)
    : QObject(parent)
{
    //get the pointer to the public class
    q_ptr = e;

    init();
}

EncoderPrivate::~EncoderPrivate()
{
}

void EncoderPrivate::setFilePath(const QString &fileName)
{
    if (m_filePath != fileName) {
        m_filePath = fileName;
    }
}

QString EncoderPrivate::filePath() const
{
    return m_filePath;
}

void EncoderPrivate::setVideoSize(const QSize &size)
{
    if (m_videoSize != size) {
        m_videoSize = size;
    }
}

QSize EncoderPrivate::videoSize() const
{
    return m_videoSize;
}

void EncoderPrivate::setFixedFrameRate(int frameRate)
{
    if (m_fixedFrameRate != frameRate) {
        m_fixedFrameRate = frameRate;
    }
}

int EncoderPrivate::fixedFrameRate() const
{
    return m_fixedFrameRate;
}

void EncoderPrivate::setEncodeAudio(bool encode)
{
    if (m_encodeAudio != encode) {
        m_encodeAudio = encode;
    }
}

bool EncoderPrivate::encodeAudio() const
{
    return m_encodeAudio;
}

void EncoderPrivate::setVideoCodec(EncoderGlobal::VideoCodec codec)
{
    if (m_videoCodecName != codec) {
        m_videoCodecName = codec;
    }
}

EncoderGlobal::VideoCodec EncoderPrivate::videoCodec() const
{
    return m_videoCodecName;
}

void EncoderPrivate::setAudioCodec(EncoderGlobal::AudioCodec codec)
{
    if (m_audioCodecName != codec) {
        m_audioCodecName = codec;
    }
}

EncoderGlobal::AudioCodec EncoderPrivate::audioCodec() const
{
    return m_audioCodecName;
}

void EncoderPrivate::setVideoCodecSettings(const VideoCodecSettings &settings)
{
    m_videoSettings = settings;
}

VideoCodecSettings EncoderPrivate::videoCodecSettings() const
{
    return m_videoSettings;
}

void EncoderPrivate::start()
{
    //check input data
    if (!videoSize().isValid()) {
        q_ptr->setError(Encoder::InvalidVideoSizeError, tr("Video size is not set."));
        return;
    }

    int width = videoSize().width();
    int height = videoSize().height();
    if ((width % 4 != 0 && width % 8 != 0 && width % 16 != 0)
            || (height % 4 != 0 && height % 8 != 0 && height % 16 != 0)) {
        q_ptr->setError(Encoder::InvalidVideoSizeError, tr("Video size dimensions must be multiple of 4,8 or 16."));
        return;
    }

    if (filePath().isEmpty()) {
        q_ptr->setError(Encoder::InvalidFilePathError, tr("File path is not set."));
        return;
    }

    //init ffmpeg stuff
    avcodec_register_all();
    av_register_all();

    m_outputFormat = av_guess_format(NULL, filePath().toLocal8Bit(), NULL);
    if (!m_outputFormat) {
        q_ptr->setError(Encoder::InvalidOutputFormatError, tr("Unable to get an output format by passed filename."));
        return;
    }

    m_formatContext = avformat_alloc_context();

    m_formatContext->oformat = m_outputFormat;

    //create streams
    if (!createVideoStream())
        return;

    //open streams
    if (!openVideoStream())
        return;

    if (avio_open(&m_formatContext->pb, filePath().toLocal8Bit(), AVIO_FLAG_WRITE) < 0) {
        q_ptr->setError(Encoder::FileOpenError, QString(tr("Unable to open: %1")).arg(filePath()));
    }

    avformat_write_header(m_formatContext, 0);
}

void EncoderPrivate::init()
{
    m_videoCodecName = EncoderGlobal::DEFAULT_VIDEO_CODEC;
    m_audioCodecName = EncoderGlobal::DEFAULT_AUDIO_CODEC;

    m_fixedFrameRate = -1;
    m_encodeAudio = true;

    //video stuff
    m_outputFormat = NULL;
    m_formatContext = NULL;
    m_videoStream = NULL;
    m_videoCodecContext = NULL;
    m_videoCodec = NULL;
    m_videoBufferSize = 0;
    m_videoBuffer = 0;

    //audio stuff
    m_audioStream = NULL;
    m_audioCodecContext = NULL;
    m_audioCodec = NULL;
    m_audioOutputBufferSize = 0;
    m_audioSampleSize = 0;
    m_audioOutputBuffer = 0;

    m_imageConvertContext = NULL;
    m_videoPicture = NULL;
}

bool EncoderPrivate::createVideoStream()
{
    m_videoStream = av_new_stream(m_formatContext,0);
    if(!m_videoStream ) {
        q_ptr->setError(Encoder::InvalidVideoStreamError, tr("Unable to add video stream."));
        return false;
    }

    //set up codec
    m_videoCodecContext = m_videoStream->codec;
    m_videoCodecContext->codec_id = (videoCodec() == EncoderGlobal::DEFAULT_VIDEO_CODEC) ? m_outputFormat->video_codec : static_cast<CodecID>(videoCodec());
    m_videoCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    m_videoCodecContext->width = videoSize().width();
    m_videoCodecContext->height = videoSize().height();
    m_videoCodecContext->pix_fmt = PIX_FMT_YUV420P;
    m_videoCodecContext->time_base.den = fixedFrameRate() != -1 ? fixedFrameRate() : 1000;
    m_videoCodecContext->time_base.num = 1;

    applyVideoCodecSettings();

    return true;
}

bool EncoderPrivate::createAudioStream()
{
    return true;
}

bool EncoderPrivate::openVideoStream()
{
    m_videoCodec = avcodec_find_encoder(m_videoCodecContext->codec_id);
    if (!m_videoCodec) {
        q_ptr->setError(Encoder::VideoEncoderNotFoundError, tr("Unable to find video encoder by codec id."));
        return false;
    }

    // open the codec
    if (avcodec_open2(m_videoCodecContext, m_videoCodec, NULL) < 0) {
        q_ptr->setError(Encoder::InvalidVideoCodecError, tr("Unable to open video codec."));
        return false;
    }

    //allocate frame buffer
    m_videoBufferSize = videoSize().width() * videoSize().height() * 1.5;
    m_videoBuffer = new uint8_t[m_videoBufferSize];

    //init frame
    m_videoPicture = avcodec_alloc_frame();

    return true;
}

bool EncoderPrivate::openAudioStream()
{
    return true;
}

void EncoderPrivate::applyVideoCodecSettings()
{
    setVideoCodecOption<int>(&AVCodecContext::bit_rate, &VideoCodecSettings::bitrate);
    setVideoCodecOption<int>(&AVCodecContext::gop_size, &VideoCodecSettings::gopSize);
    setVideoCodecOption<int>(&AVCodecContext::qmin, &VideoCodecSettings::minimumQuantizer);
    setVideoCodecOption<int>(&AVCodecContext::qmax, &VideoCodecSettings::minimumQuantizer);
    setVideoCodecOption<int>(&AVCodecContext::max_qdiff, &VideoCodecSettings::maximumQuantizerDifference);
    setVideoCodecOption<int>(&AVCodecContext::coder_type, reinterpret_cast<int (VideoCodecSettings::*) () const>(&VideoCodecSettings::coderType));
    setVideoCodecOption<int>(&AVCodecContext::me_cmp, &VideoCodecSettings::motionEstimationComparison);
    setVideoCodecOption<int>(&AVCodecContext::partitions, reinterpret_cast<int (VideoCodecSettings::*) () const>(&VideoCodecSettings::partitions));
    setVideoCodecOption<int>(&AVCodecContext::me_method, reinterpret_cast<int (VideoCodecSettings::*) () const>(&VideoCodecSettings::motionEstimationMethod));
    setVideoCodecOption<int>(&AVCodecContext::me_subpel_quality, &VideoCodecSettings::subpixelMotionEstimationQuality);
    setVideoCodecOption<int>(&AVCodecContext::me_range, &VideoCodecSettings::motionEstimationRange);
    setVideoCodecOption<int>(&AVCodecContext::keyint_min, &VideoCodecSettings::minimumKeyframeInterval);
    setVideoCodecOption<int>(&AVCodecContext::scenechange_threshold, &VideoCodecSettings::sceneChangeThreshold);
    setVideoCodecOption<float>(&AVCodecContext::i_quant_factor, &VideoCodecSettings::iQuantFactor);
    setVideoCodecOption<int>(&AVCodecContext::b_frame_strategy, &VideoCodecSettings::bFrameStrategy);
    setVideoCodecOption<float>(&AVCodecContext::qcompress, &VideoCodecSettings::quantizerCurveCompressionFactor);
    setVideoCodecOption<int>(&AVCodecContext::max_b_frames, &VideoCodecSettings::maximumBFrames);
    setVideoCodecOption<int>(&AVCodecContext::refs, &VideoCodecSettings::referenceFrameCount);
    setVideoCodecOption<int>(&AVCodecContext::directpred, reinterpret_cast<int (VideoCodecSettings::*) () const>(&VideoCodecSettings::directMvPredictionMode));
    setVideoCodecOption<int>(&AVCodecContext::trellis, &VideoCodecSettings::trellis);
    setVideoCodecOption<int>(&AVCodecContext::weighted_p_pred, reinterpret_cast<int (VideoCodecSettings::*) () const>(&VideoCodecSettings::pFramePredictionAnalysisMethod));
    setVideoCodecOption<int>(&AVCodecContext::rc_lookahead, &VideoCodecSettings::rcLookahead);
    setVideoCodecOption<int>(&AVCodecContext::flags, reinterpret_cast<int (VideoCodecSettings::*) () const>(&VideoCodecSettings::flags));
    setVideoCodecOption<int>(&AVCodecContext::flags2, reinterpret_cast<int (VideoCodecSettings::*) () const>(&VideoCodecSettings::flags2));
}

template <class T>
void EncoderPrivate::setVideoCodecOption(T AVCodecContext::*option, T (VideoCodecSettings::*f)() const)
{
    T value = (m_videoSettings.*f)();
    if (value != -1) {
        m_videoCodecContext->*option = (m_videoSettings.*f)();
    }
}


//---------------------------------------------------------------------------------
// Encoder implementation
//---------------------------------------------------------------------------------


Encoder::Encoder(QObject *parent) :
    QObject(parent)
  , d_ptr(new EncoderPrivate(this))
  , m_encoderThread(new QThread(this))
  , m_error(Encoder::NoError)
{
    d_ptr->moveToThread(m_encoderThread);
}

Encoder::~Encoder()
{
    delete d_ptr;
}

void Encoder::setFilePath(const QString &filePath)
{
    d_ptr->setFilePath(filePath);
}

QString Encoder::filePath() const
{
    return d_ptr->filePath();
}

void Encoder::setVideoSize(const QSize &size)
{
    d_ptr->setVideoSize(size);
}

QSize Encoder::videoSize() const
{
    return d_ptr->videoSize();
}

void Encoder::setFixedFrameRate(int frameRate)
{
    d_ptr->setFixedFrameRate(frameRate);
}

int Encoder::fixedFrameRate() const
{
    return d_ptr->fixedFrameRate();
}

void Encoder::setEncodeAudio(bool encode)
{
    d_ptr->setEncodeAudio(encode);
}

bool Encoder::encodeAudio() const
{
    return d_ptr->encodeAudio();
}

void Encoder::setVideoCodec(EncoderGlobal::VideoCodec codec)
{
    d_ptr->setVideoCodec(codec);
}

EncoderGlobal::VideoCodec Encoder::videoCodec() const
{
    return d_ptr->videoCodec();
}

void Encoder::setAudioCodec(EncoderGlobal::AudioCodec codec)
{
    d_ptr->setAudioCodec(codec);
}

EncoderGlobal::AudioCodec Encoder::audioCodec() const
{
    return d_ptr->audioCodec();
}

void Encoder::setVideoCodecSettings(const VideoCodecSettings &settings)
{
    d_ptr->setVideoCodecSettings(settings);
}

VideoCodecSettings Encoder::videoCodecSettings() const
{
    return d_ptr->videoCodecSettings();
}

Encoder::Error Encoder::error() const
{
    return m_error;
}

QString Encoder::errorString() const
{
    return m_errorString;
}

void Encoder::setError(Encoder::Error errorCode, const QString &errorString)
{
    m_error = errorCode;
    m_errorString = errorString;

    emit error(errorCode);
}

#include "encoder.moc"
