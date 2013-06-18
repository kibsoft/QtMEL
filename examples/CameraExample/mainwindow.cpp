#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QUrl>
#include <QImage>
#include <QDebug>

#include <Recorder>
#include <CameraGrabber>
#include <AudioGrabber>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , m_recorder(new Recorder(this))
  , m_cameraGrabber(new CameraGrabber(this))
  , m_audioGrabber(new AudioGrabber(this))
{
    ui->setupUi(this);

    //setup the camera grabber
    m_cameraGrabber->setDeviceIndex(0);
    m_cameraGrabber->setLatency(65);
    m_cameraGrabber->setSize(QSize(640, 360));
    m_recorder->setImageGrabber(m_cameraGrabber);

    //setup the audio grabber
    AudioFormat format;
    format.setChannelCount(2);
    format.setSampleRate(44100);
    format.setFormat(AudioFormat::SignedInt16);

    m_audioGrabber->setDeviceIndex(0);
    m_audioGrabber->setFormat(format);
    m_recorder->setAudioGrabber(m_audioGrabber);

    //setup the encoder
    m_recorder->encoder()->setVideoCodecSettings(videoCodecSettings());
    m_recorder->encoder()->setAudioCodecSettings(audioCodecSettings());
    m_recorder->encoder()->setVideoCodec(EncoderGlobal::H264);
    m_recorder->encoder()->setAudioCodec(EncoderGlobal::MP3);
    m_recorder->encoder()->setOutputPixelFormat(EncoderGlobal::YUV420P);
//    m_recorder->encoder()->setVideoSize(m_cameraGrabber->size());
    m_recorder->encoder()->setVideoSize(m_cameraGrabber->size());

    initFrameLabel();

    connect(m_recorder->encoder(), SIGNAL(error(Encoder::Error)), this, SLOT(onEncoderError(Encoder::Error)));
    connect(m_cameraGrabber, SIGNAL(error(AbstractGrabber::Error)), this, SLOT(onGrabberError(AbstractGrabber::Error)));
    connect(m_audioGrabber, SIGNAL(error(AbstractGrabber::Error)), this, SLOT(onGrabberError(AbstractGrabber::Error)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Choose an output file"), QString(), tr("Videos (*.avi)"));
    if (!fileName.isEmpty()) {
        ui->pathValueLabel->setText(fileName);
        ui->pathValueLabel->setToolTip(fileName);
        ui->startButton->setEnabled(true);
        m_recorder->encoder()->setFilePath(ui->pathValueLabel->text());
    }
}

void MainWindow::on_startButton_clicked()
{
    connect(m_cameraGrabber, SIGNAL(frameAvailable(QImage,int)), this, SLOT(showFrame(QImage)));
    m_recorder->start();

    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
}

void MainWindow::on_pauseButton_clicked()
{
    m_recorder->pause();

    ui->pauseButton->setEnabled(false);
    ui->resumeButton->setEnabled(true);
}

void MainWindow::on_resumeButton_clicked()
{
    m_recorder->resume();

    ui->resumeButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
}

void MainWindow::on_stopButton_clicked()
{
    disconnect(m_cameraGrabber, SIGNAL(frameAvailable(QImage,int)), this, SLOT(showFrame(QImage)));
    m_recorder->stop();

    ui->startButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    ui->resumeButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    initFrameLabel();

    QDesktopServices::openUrl(QUrl(ui->pathValueLabel->text()));
}

void MainWindow::showFrame(const QImage &frame)
{
    ui->frameLabel->setPixmap(QPixmap::fromImage(frame));
}

void MainWindow::onEncoderError(Encoder::Error error)
{
    Q_UNUSED(error)

    qDebug()<<"Encoder's error number: "<<error;
}

void MainWindow::onGrabberError(AbstractGrabber::Error error)
{
    Q_UNUSED(error)

    qDebug()<<"Grabber's error number: "<<error;
}

void MainWindow::initFrameLabel()
{
    QImage blackImage(640, 480, QImage::Format_RGB888);
    blackImage.fill(Qt::black);

    ui->frameLabel->setPixmap(QPixmap::fromImage(blackImage));
    ui->frameLabel->repaint();
}

VideoCodecSettings MainWindow::videoCodecSettings() const
{
    //x264 loseless fast preset
    VideoCodecSettings settings;
    settings.setCoderType(EncoderGlobal::Vlc);
    settings.setFlags(EncoderGlobal::LoopFilter);
    settings.setMotionEstimationComparison(1);
    settings.setPartitions(EncoderGlobal::I4x4 | EncoderGlobal::P8x8);
    settings.setMotionEstimationMethod(EncoderGlobal::Hex);
    settings.setSubpixelMotionEstimationQuality(3);
    settings.setMotionEstimationRange(16);
    settings.setGopSize(250);
    settings.setMinimumKeyframeInterval(25);
    settings.setSceneChangeThreshold(40);
    settings.setIQuantFactor(0.71f);
    settings.setBFrameStrategy(1);
    settings.setQuantizerCurveCompressionFactor(0.6f);
    settings.setMinimumQuantizer(0);
    settings.setMaximumQuantizer(69);
    settings.setMaximumQuantizerDifference(4);
    settings.setDirectMvPredictionMode(EncoderGlobal::SpatialMode);
    settings.setFlags2(EncoderGlobal::FastPSkip);
    settings.setConstantQuantizerMode(0);
    settings.setPFramePredictionAnalysisMethod(EncoderGlobal::NoWpm);

    return settings;
}

AudioCodecSettings MainWindow::audioCodecSettings() const
{
    AudioCodecSettings settings;
    settings.setSampleRate(m_audioGrabber->format().sampleRate());
    settings.setChannelCount(m_audioGrabber->format().channelCount());
    settings.setSampleFormat(EncoderGlobal::Signed16);

    return settings;
}

void MainWindow::on_muteButton_clicked()
{
    m_recorder->mute();
}

void MainWindow::on_unmuteButton_clicked()
{
    m_recorder->unmute();
}
