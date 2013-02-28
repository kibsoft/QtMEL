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

typedef uint8_t AVBuffer;

class EncoderObject : public QObject {
    Q_OBJECT

public:
    explicit EncoderObject(QObject *parent = 0);
    ~EncoderObject();

    bool setFileName(const QString &fileName);
    QString fileName() const;

    bool setVideoSize(const QSize &size);
    QSize videoSize() const;

    bool setFixedFrameRate(int frameRate);
    int fixedFrameRate() const;

    bool setEncodeAudio(bool encode);
    bool encodeAudio() const;

public slots:
    void start();

private:
    void init();

    QString m_fileName;
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
    AVBuffer *m_pictureBuffer;
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

EncoderObject::EncoderObject(QObject *parent)
    : QObject(parent)
{
    init();
}

EncoderObject::~EncoderObject()
{
}

bool EncoderObject::setFileName(const QString &fileName)
{
    if (m_fileName != fileName) {
        m_fileName = fileName;

        return true;
    }

    return false;
}

QString EncoderObject::fileName() const
{
    return m_fileName;
}

bool EncoderObject::setVideoSize(const QSize &size)
{
    if (m_videoSize != size) {
        m_videoSize = size;

        return true;
    }

    return false;
}

QSize EncoderObject::videoSize() const
{
    return m_videoSize;
}

bool EncoderObject::setFixedFrameRate(int frameRate)
{
    if (m_fixedFrameRate != frameRate) {
        m_fixedFrameRate = frameRate;

        return true;
    }

    return false;
}

int EncoderObject::fixedFrameRate() const
{
    return m_fixedFrameRate;
}

bool EncoderObject::setEncodeAudio(bool encode)
{
    if (m_encodeAudio != encode) {
        m_encodeAudio = encode;

        return true;
    }

    return false;
}

bool EncoderObject::encodeAudio() const
{
    return m_encodeAudio;
}

void EncoderObject::start()
{
    //init ffmpeg stuff
    avcodec_register_all();
    av_register_all();
}

void EncoderObject::init()
{
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
    m_pictureBuffer = 0;
}

Encoder::Encoder(QObject *parent) :
    QObject(parent)
  , m_encoder(new EncoderObject)
{

}

Encoder::~Encoder()
{
    delete m_encoder;
}

void Encoder::setFileName(const QString &fileName)
{
    if (m_encoder->setFileName(fileName))
        emit fileNameChanged(fileName);
}

QString Encoder::fileName() const
{
    return m_encoder->fileName();
}

void Encoder::setVideoSize(const QSize &size)
{
    if (m_encoder->setVideoSize(size))
        emit videoSizeChanged(size);
}

QSize Encoder::videoSize() const
{
    return m_encoder->videoSize();
}

void Encoder::setFixedFrameRate(int frameRate)
{
    if (m_encoder->setFixedFrameRate(frameRate))
        emit fixedFrameRateChanged(frameRate);
}

int Encoder::fixedFrameRate() const
{
    return m_encoder->fixedFrameRate();
}

void Encoder::setEncodeAudio(bool encode)
{
    if (m_encoder->setEncodeAudio(encode))
        emit encodeAudioChanged(encode);
}

bool Encoder::encodeAudio() const
{
    return m_encoder->encodeAudio();
}

#include "encoder.moc"
