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

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QSize videoSize READ videoSize WRITE setVideoSize NOTIFY videoSizeChanged)
    Q_PROPERTY(int fixedFrameRate READ fixedFrameRate WRITE setFixedFrameRate NOTIFY fixedFrameRateChanged)
    Q_PROPERTY(bool encodeAudio READ encodeAudio WRITE setEncodeAudio NOTIFY encodeAudioChanged)

public:
    explicit Encoder(QObject *parent = 0);
    ~Encoder();

    void setFileName(const QString &fileName);
    QString fileName() const;

    void setVideoSize(const QSize &size);
    QSize videoSize() const;

    void setFixedFrameRate(int frameRate);
    int fixedFrameRate() const;

    void setEncodeAudio(bool encode);
    bool encodeAudio() const;

signals:
    void fileNameChanged(const QString &fileName);
    void videoSizeChanged(const QSize &size);
    void fixedFrameRateChanged(int frameRate);
    void encodeAudioChanged(bool encode);

private:
    EncoderObject *m_encoder;
    QThread *m_encoderThread;
};

#endif // ENCODER_H
