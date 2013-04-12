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

#include <QMetaType>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QImage>

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
    bool isFixedFrameRate() const;

    void setEncodeAudio(bool encode);
    bool encodeAudio() const;

    void setOutputPixelFormat(EncoderGlobal::PixelFormat format);
    EncoderGlobal::PixelFormat outputPixelFormat() const;

    void setVideoCodec(EncoderGlobal::VideoCodec codec);
    EncoderGlobal::VideoCodec videoCodec() const;

    void setAudioCodec(EncoderGlobal::AudioCodec codec);
    EncoderGlobal::AudioCodec audioCodec() const;

    void setVideoCodecSettings(const VideoCodecSettings &settings);
    VideoCodecSettings videoCodecSettings() const;

    int encodedFrameCount() const;

public slots:
    void start();
    void stop();

    void encodeVideoFrame(const QImage &frame, int duration);

private slots:
    void onError();

private:
    void initData();
    void initFfmpegStuff();
    void cleanup();

    bool createVideoStream();
    bool createAudioStream();

    bool openVideoStream();
    bool openAudioStream();

    bool convertImage(const QImage &image);
    EncoderGlobal::PixelFormat convertImagePixelFormat(QImage::Format format) const;

    void applyVideoCodecSettings();
    template <class T1, class T2> void setVideoCodecOption(T1 AVCodecContext::*option, T2 (VideoCodecSettings::*f)() const);

    Encoder *q_ptr;

    EncoderGlobal::PixelFormat m_outputPixelFormat;
    EncoderGlobal::VideoCodec m_videoCodecName;
    EncoderGlobal::AudioCodec m_audioCodecName;
    VideoCodecSettings m_videoSettings;

    QString m_filePath;
    QSize m_videoSize;
    int m_fixedFrameRate;
    bool m_encodeAudio;

    int m_prevFrameDuration;
    int m_pts;
    int m_encodedFrameCount;

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
    AVBuffer *m_pictureBuffer;

    //audio stuff
    AVStream *m_audioStream;
    AVCodecContext *m_audioCodecContext;
    AVCodec *m_audioCodec;
    AVBuffer *m_audioOutputBuffer;
    QByteArray m_audioInputBuffer;
    int m_audioOutputBufferSize;
    int m_audioSampleSize;

    mutable QMutex m_encodedFrameCountMutex;
};

EncoderPrivate::EncoderPrivate(Encoder *e, QObject *parent)
    : QObject(parent)
{
    //get the pointer to the public class
    q_ptr = e;

    initData();
    initFfmpegStuff();

    //cleanup ffmpeg stuff on error
    connect(q_ptr, SIGNAL(error(Encoder::Error)), this, SLOT(onError()));
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

bool EncoderPrivate::isFixedFrameRate() const
{
    return fixedFrameRate() != -1;
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

void EncoderPrivate::setOutputPixelFormat(EncoderGlobal::PixelFormat format)
{
    if (m_outputPixelFormat != format) {
        m_outputPixelFormat = format;
    }
}

EncoderGlobal::PixelFormat EncoderPrivate::outputPixelFormat() const
{
    return m_outputPixelFormat;
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

int EncoderPrivate::encodedFrameCount() const
{
    QMutexLocker locker(&m_encodedFrameCountMutex);
    return m_encodedFrameCount;
}

void EncoderPrivate::start()
{
    //check input data
    if (!(videoSize().width() > 0 && videoSize().height() > 0)) {
        q_ptr->setError(Encoder::InvalidVideoSizeError, tr("Video size is invalid. Width and height must be greater than 0."));
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

    m_outputFormat = av_guess_format(NULL, filePath().toUtf8().constData(), NULL);
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

    if (avio_open(&m_formatContext->pb, filePath().toUtf8().constData(), AVIO_FLAG_WRITE) < 0) {
        q_ptr->setError(Encoder::FileOpenError, QString(tr("Unable to open: %1")).arg(filePath()));
        return;
    }

    avformat_write_header(m_formatContext, 0);

    q_ptr->setState(Encoder::ActiveState);
}

void EncoderPrivate::stop()
{
    q_ptr->setState(Encoder::StoppedState);

    av_write_trailer(m_formatContext);

    cleanup();
}

void EncoderPrivate::encodeVideoFrame(const QImage &frame, int duration)
{
    if (duration) {//if duration is -1(fixed fps) or greater than 0
        if (convertImage(frame)) {
            int outSize = avcodec_encode_video(m_videoCodecContext, m_videoBuffer, m_videoBufferSize, m_videoPicture);

            if (outSize > 0) {
                if (!isFixedFrameRate()) {
                    m_pts += m_prevFrameDuration;
                    m_prevFrameDuration = duration;

                    m_videoCodecContext->coded_frame->pts = m_pts;
                }

                AVPacket pkt;
                av_init_packet(&pkt);

                pkt.pts = m_videoCodecContext->coded_frame->pts;

                if(m_videoCodecContext->coded_frame->key_frame)
                    pkt.flags |= AV_PKT_FLAG_KEY;

                pkt.stream_index = m_videoStream->index;
                pkt.data = m_videoBuffer;
                pkt.size = outSize;
                av_interleaved_write_frame(m_formatContext, &pkt);

                QMutexLocker locker(&m_encodedFrameCountMutex);
                ++m_encodedFrameCount;
            }
        }
    }
}

void EncoderPrivate::onError()
{
    q_ptr->setState(Encoder::StoppedState);

    cleanup();
}

void EncoderPrivate::initData()
{
    m_outputPixelFormat = EncoderGlobal::PIXEL_FORMAT_NONE;
    m_videoCodecName = EncoderGlobal::DEFAULT_VIDEO_CODEC;
    m_audioCodecName = EncoderGlobal::DEFAULT_AUDIO_CODEC;

    m_fixedFrameRate = -1;
    m_encodeAudio = true;
}


void EncoderPrivate::initFfmpegStuff()
{
    m_prevFrameDuration = 0;
    m_pts = 0;
    m_encodedFrameCount = 0;

    //video stuff
    m_outputFormat = NULL;
    m_formatContext = NULL;
    m_videoStream = NULL;
    m_videoCodecContext = NULL;
    m_videoCodec = NULL;
    m_videoBufferSize = 0;
    m_videoBuffer = 0;
    m_pictureBuffer = 0;

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

void EncoderPrivate::cleanup()
{
    //close codecs
    if (m_videoCodecContext != NULL)
        avcodec_close(m_videoCodecContext);

    if (m_audioCodecContext != NULL)
        avcodec_close(m_audioCodecContext);

    //remove subsidiary objects
    if (m_videoBuffer)
        delete[] m_videoBuffer;

    if (m_pictureBuffer)
        delete[] m_pictureBuffer;

    if (m_audioOutputBuffer)
        delete[] m_audioOutputBuffer;

    if (m_videoPicture != NULL)
        av_free(m_videoPicture);

    if (m_formatContext != NULL) {
        //remove ffmpeg objects
        for (unsigned i = 0; i < m_formatContext->nb_streams; i++) {
            av_freep(&m_formatContext->streams[i]->codec);
            av_freep(&m_formatContext->streams[i]);
        }

        avio_close(m_formatContext->pb);

        av_free(m_formatContext);
    }

    initFfmpegStuff();
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
    m_videoCodecContext->pix_fmt = (PixelFormat)outputPixelFormat();
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

    int size = avpicture_get_size(m_videoCodecContext->pix_fmt, m_videoCodecContext->width, m_videoCodecContext->height);
    m_pictureBuffer = new uint8_t[size];

    // Setup the planes
    avpicture_fill((AVPicture *)m_videoPicture, m_pictureBuffer,m_videoCodecContext->pix_fmt, m_videoCodecContext->width, m_videoCodecContext->height);

    return true;
}

bool EncoderPrivate::openAudioStream()
{
    return true;
}

bool EncoderPrivate::convertImage(const QImage &image)
{
    EncoderGlobal::PixelFormat inputFormat = convertImagePixelFormat(image.format());
    if (inputFormat == EncoderGlobal::PIXEL_FORMAT_NONE) {
        q_ptr->setError(Encoder::InvalidInputPixelFormat, tr("Could not convert input pixel format to the ffmpeg's format."));
        return false;
    }

    m_imageConvertContext = sws_getCachedContext(m_imageConvertContext, image.width(), image.height(),
                                                 (PixelFormat)inputFormat, image.width(), image.height(), m_videoCodecContext->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);

    if (m_imageConvertContext == NULL) {
        q_ptr->setError(Encoder::InvalidConversionContext, tr("Could not initialize conversion context."));
        return false;
    }

    uint8_t *srcplanes[3];
    srcplanes[0]=(uint8_t*)image.bits();
    srcplanes[1]=0;
    srcplanes[2]=0;

    int srcstride[3];
    srcstride[0]=image.bytesPerLine();
    srcstride[1]=0;
    srcstride[2]=0;

    sws_scale(m_imageConvertContext, srcplanes, srcstride,0, image.height(), m_videoPicture->data, m_videoPicture->linesize);

    return true;
}

EncoderGlobal::PixelFormat EncoderPrivate::convertImagePixelFormat(QImage::Format format) const
{
    EncoderGlobal::PixelFormat newFormat;

    switch (format) {
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    case QImage::Format_RGB32:
        newFormat = EncoderGlobal::BGRA;
        break;

    case QImage::Format_RGB16:
        newFormat = EncoderGlobal::RGB565LE;
        break;

    case QImage::Format_RGB888:
        newFormat = EncoderGlobal::RGB24;
        break;

    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
        newFormat = EncoderGlobal::MONOWHITE;
        break;

    default:
        newFormat = EncoderGlobal::PIXEL_FORMAT_NONE;
        break;
    }

    return newFormat;
}

void EncoderPrivate::applyVideoCodecSettings()
{
    setVideoCodecOption<int, int>(&AVCodecContext::bit_rate, &VideoCodecSettings::bitrate);
    setVideoCodecOption<int, int>(&AVCodecContext::gop_size, &VideoCodecSettings::gopSize);
    setVideoCodecOption<int, int>(&AVCodecContext::qmin, &VideoCodecSettings::minimumQuantizer);
    setVideoCodecOption<int, int>(&AVCodecContext::qmax, &VideoCodecSettings::minimumQuantizer);
    setVideoCodecOption<int, int>(&AVCodecContext::max_qdiff, &VideoCodecSettings::maximumQuantizerDifference);
    setVideoCodecOption<int, EncoderGlobal::CoderType>(&AVCodecContext::coder_type, &VideoCodecSettings::coderType);
    setVideoCodecOption<int, int>(&AVCodecContext::me_cmp, &VideoCodecSettings::motionEstimationComparison);
    setVideoCodecOption<int, EncoderGlobal::Partitions>(&AVCodecContext::partitions, &VideoCodecSettings::partitions);
    setVideoCodecOption<int, EncoderGlobal::MotionEstimationAlgorithm>(&AVCodecContext::me_method, &VideoCodecSettings::motionEstimationMethod);
    setVideoCodecOption<int, int>(&AVCodecContext::me_subpel_quality, &VideoCodecSettings::subpixelMotionEstimationQuality);
    setVideoCodecOption<int, int>(&AVCodecContext::me_range, &VideoCodecSettings::motionEstimationRange);
    setVideoCodecOption<int, int>(&AVCodecContext::keyint_min, &VideoCodecSettings::minimumKeyframeInterval);
    setVideoCodecOption<int, int>(&AVCodecContext::scenechange_threshold, &VideoCodecSettings::sceneChangeThreshold);
    setVideoCodecOption<float, float>(&AVCodecContext::i_quant_factor, &VideoCodecSettings::iQuantFactor);
    setVideoCodecOption<int, int>(&AVCodecContext::b_frame_strategy, &VideoCodecSettings::bFrameStrategy);
    setVideoCodecOption<float, float>(&AVCodecContext::qcompress, &VideoCodecSettings::quantizerCurveCompressionFactor);
    setVideoCodecOption<int, int>(&AVCodecContext::max_b_frames, &VideoCodecSettings::maximumBFrames);
    setVideoCodecOption<int, int>(&AVCodecContext::refs, &VideoCodecSettings::referenceFrameCount);
    setVideoCodecOption<int, EncoderGlobal::MotionVectorPredictionMode>(&AVCodecContext::directpred, &VideoCodecSettings::directMvPredictionMode);
    setVideoCodecOption<int, int>(&AVCodecContext::trellis, &VideoCodecSettings::trellis);
    setVideoCodecOption<int, EncoderGlobal::WeightedPredictionMethod>(&AVCodecContext::weighted_p_pred, &VideoCodecSettings::pFramePredictionAnalysisMethod);
    setVideoCodecOption<int, int>(&AVCodecContext::rc_lookahead, &VideoCodecSettings::rcLookahead);
    setVideoCodecOption<int, EncoderGlobal::Flags>(&AVCodecContext::flags, &VideoCodecSettings::flags);
    setVideoCodecOption<int, EncoderGlobal::Flags2>(&AVCodecContext::flags2, &VideoCodecSettings::flags2);
}

template <class T1, class T2>
void EncoderPrivate::setVideoCodecOption(T1 AVCodecContext::*option, T2 (VideoCodecSettings::*f)() const)
{
    T2 value = (m_videoSettings.*f)();
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
  , m_state(Encoder::StoppedState)
  , m_error(Encoder::NoError)
{
    qRegisterMetaType<Encoder::Error>("Encoder::Error");
    qRegisterMetaType<Encoder::State>("Encoder::State");

    d_ptr->moveToThread(m_encoderThread);
    m_encoderThread->start();
}

Encoder::~Encoder()
{
    m_encoderThread->terminate();
    delete d_ptr;
}

void Encoder::setFilePath(const QString &filePath)
{
    if (state() != Encoder::ActiveState)
        d_ptr->setFilePath(filePath);
}

QString Encoder::filePath() const
{
    return d_ptr->filePath();
}

void Encoder::setVideoSize(const QSize &size)
{
    if (state() != Encoder::ActiveState)
        d_ptr->setVideoSize(size);
}

QSize Encoder::videoSize() const
{
    return d_ptr->videoSize();
}

void Encoder::setFixedFrameRate(int frameRate)
{
    if (state() != Encoder::ActiveState)
        d_ptr->setFixedFrameRate(frameRate);
}

int Encoder::fixedFrameRate() const
{
    return d_ptr->fixedFrameRate();
}

void Encoder::setEncodeAudio(bool encode)
{
    if (state() != Encoder::ActiveState)
        d_ptr->setEncodeAudio(encode);
}

bool Encoder::encodeAudio() const
{
    return d_ptr->encodeAudio();
}

void Encoder::setOutputPixelFormat(EncoderGlobal::PixelFormat format)
{
    if (state() != Encoder::ActiveState)
        d_ptr->setOutputPixelFormat(format);
}

EncoderGlobal::PixelFormat Encoder::outputPixelFormat() const
{
    return d_ptr->outputPixelFormat();
}

void Encoder::setVideoCodec(EncoderGlobal::VideoCodec codec)
{
    if (state() != Encoder::ActiveState)
        d_ptr->setVideoCodec(codec);
}

EncoderGlobal::VideoCodec Encoder::videoCodec() const
{
    return d_ptr->videoCodec();
}

void Encoder::setAudioCodec(EncoderGlobal::AudioCodec codec)
{
    if (state() != Encoder::ActiveState)
        d_ptr->setAudioCodec(codec);
}

EncoderGlobal::AudioCodec Encoder::audioCodec() const
{
    return d_ptr->audioCodec();
}

void Encoder::setVideoCodecSettings(const VideoCodecSettings &settings)
{
    if (state() != Encoder::ActiveState)
        d_ptr->setVideoCodecSettings(settings);
}

VideoCodecSettings Encoder::videoCodecSettings() const
{
    return d_ptr->videoCodecSettings();
}

int Encoder::encodedFrameCount() const
{
    return d_ptr->encodedFrameCount();
}

Encoder::State Encoder::state() const
{
    return m_state;
}

Encoder::Error Encoder::error() const
{
    return m_error;
}

void Encoder::setState(Encoder::State state)
{
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }
}

QString Encoder::errorString() const
{
    return m_errorString;
}

void Encoder::start()
{
    if (state() != Encoder::ActiveState)
        QMetaObject::invokeMethod(d_ptr, "start", Qt::QueuedConnection);
}

void Encoder::stop()
{
    if (state() == Encoder::ActiveState)
        QMetaObject::invokeMethod(d_ptr, "stop", Qt::QueuedConnection);
}

void Encoder::encodeVideoFrame(const QImage &frame, int duration)
{
    if (state() == Encoder::ActiveState) {
        QMetaObject::invokeMethod(d_ptr, "encodeVideoFrame", Qt::QueuedConnection,
                                  Q_ARG(QImage, frame),
                                  Q_ARG(int, duration));
    }
}

void Encoder::setError(Encoder::Error errorCode, const QString &errorString)
{
    m_error = errorCode;
    m_errorString = errorString;

    emit error(errorCode);
}

#include "encoder.moc"
