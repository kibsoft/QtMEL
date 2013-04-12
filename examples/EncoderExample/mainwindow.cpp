#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QImage>
#include <QPainter>
#include <QMetaObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , m_screenGrabber(new ScreenGrabber(this))
  , m_encoder(new Encoder(this))
{
    ui->setupUi(this);

    Q_ASSERT(connect(m_encoder, SIGNAL(error(Encoder::Error)), this, SLOT(onEncoderError(Encoder::Error))));
    Q_ASSERT(connect(m_encoder, SIGNAL(stateChanged(Encoder::State)), this, SLOT(onState(Encoder::State))));
    Q_ASSERT(connect(m_screenGrabber, SIGNAL(frameAvailable(QImage,int)), m_encoder, SLOT(encodeVideoFrame(QImage,int))));
    Q_ASSERT(connect(m_screenGrabber, SIGNAL(error(AbstractGrabber::Error)), this, SLOT(onImageGrabberError(AbstractGrabber::Error))));

    m_encoder->setVideoCodecSettings(videoCodecSettings());
    m_encoder->setVideoCodec(EncoderGlobal::H264);
    m_encoder->setOutputPixelFormat(EncoderGlobal::YUV420P);

    m_screenGrabber->setLatency(40);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
}

void MainWindow::onEncoderError(Encoder::Error error)
{
    Q_UNUSED(error)

    showMessage(tr("Encoder error"), m_encoder->errorString());
}

void MainWindow::onImageGrabberError(AbstractGrabber::Error error)
{
    Q_UNUSED(error)

    showMessage(tr("Screen grabber error"), m_screenGrabber->errorString());
}

void MainWindow::onState(Encoder::State state)
{
    if (state == Encoder::ActiveState) {
        //if screen recording example
        if (ui->screenRecrdGB->isEnabled()) {
            if (!m_screenGrabber->start()) {
                m_encoder->stop();
                return;
            }

            ui->StartButton->setEnabled(false);
            ui->StopButton->setEnabled(true);
            ui->fixedFPSGB->setEnabled(false);
        } else {//if fixed fps example
            generateFrames();
        }
    } else {
        //if fixed fps example
        if (!ui->screenRecrdGB->isEnabled()) {
            //unlock ui
            ui->fixedFPSStart->setEnabled(true);
            ui->screenRecrdGB->setEnabled(true);

            //play final video
            QDesktopServices::openUrl(qApp->applicationDirPath() + "/fixed_fps_video.avi");
        }
    }
}

void MainWindow::on_chooseFileButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Choose file..."), qApp->applicationDirPath(), tr("Videos (*.avi *.mp4)"));
    if (!fileName.isEmpty()) {
        ui->filePathLabel->setText(fileName);
        ui->filePathLabel->setToolTip(fileName);
    }
}

void MainWindow::on_StartButton_clicked()
{
    QString filePath = ui->filePathLabel->text() == tr("None") ? "" : ui->filePathLabel->text();
    m_encoder->setFilePath(filePath);

    QRect captureRect(ui->xSpinBox->value(), ui->ySpinBox->value(), ui->widthSpinBox->value(), ui->heightSpinBox->value());
    m_screenGrabber->setCaptureRect(captureRect);
    m_encoder->setVideoSize(captureRect.size());

    m_encoder->start();
}

void MainWindow::showMessage(const QString &title, const QString &message)
{
    QMessageBox::critical(this, title, message);
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

void MainWindow::on_StopButton_clicked()
{
    m_screenGrabber->stop();
    m_encoder->stop();

    ui->StartButton->setEnabled(true);
    ui->StopButton->setEnabled(false);
    ui->fixedFPSGB->setEnabled(true);

    if (ui->playVideoCheckBox->isChecked()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(ui->filePathLabel->text()));
    }
}

void MainWindow::on_fixedFPSStart_clicked()
{
    //lock ui
    ui->fixedFPSStart->setEnabled(false);
    ui->screenRecrdGB->setEnabled(false);

    //setup encoder
    m_encoder->setFilePath(qApp->applicationDirPath() + "/fixed_fps_video.avi");
    m_encoder->setVideoSize(QSize(1024, 768));
    m_encoder->setFixedFrameRate(ui->fpsSpinBox->value());
    m_encoder->start();
}

void MainWindow::generateFrames()
{
    //generate 500 frames
    for (int i = 0; i < 500; ++i) {
        QImage frame(QSize(1024, 768), QImage::Format_RGB16);
        frame.fill(Qt::yellow);
        QPainter painter(&frame);
        painter.setPen(QPen(Qt::red, 3));
        painter.drawRect(i, i, 30, 30);
        ui->statusBar->showMessage(QString::number(i/5) + "%");
        m_encoder->encodeVideoFrame(frame);

        //this is needed to prevent RAM overflow on some computers
        if (i % 20 == 0) {
            while (m_encoder->encodedFrameCount() != i) { qApp->processEvents(); }
        }

        qApp->processEvents();
    }

    ui->statusBar->clearMessage();
    m_encoder->stop();
}
