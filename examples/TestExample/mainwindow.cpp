#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QTimer>
#include <ScreenGrabber>
#include <AudioGrabber>
#include <QAudioDeviceInfo>
#include <QtCore/qendian.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_helper(new MouseHelper(this))
  , m_screenGrabber(new ScreenGrabber(this))
  , m_audioGrabber(new AudioGrabber(this))
  , m_maxAmplitude(0)
  , m_grabbingTime(0)
  , m_grabbedFrameCount(0)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowStaysOnTopHint);

    //set up gui
    QRect fullScreenRect = qApp->desktop()->screen()->rect();

    ui->widthSpinBox->setMaximum(fullScreenRect.width());
    ui->heightSpinBox->setMaximum(fullScreenRect.height());

    ui->widthSpinBox->setValue(fullScreenRect.width());
    ui->heightSpinBox->setValue(fullScreenRect.height());

    //setup timer
    m_cursorTimer.setInterval(50);

    //connect signals
    connect(m_helper, SIGNAL(mouseEvent(MouseEvent)), this, SLOT(updateMHStatusLabel(MouseEvent)));
    connect(m_screenGrabber, SIGNAL(frameAvailable(QImage,int)), this, SLOT(updateAverageFrameRate(QImage,int)));
    connect(m_audioGrabber, SIGNAL(frameAvailable(QByteArray)), this, SLOT(updateSoundPB(QByteArray)));
    connect(&m_cursorTimer, SIGNAL(timeout()), this, SLOT(updateCursorLabel()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_screenGrabber->state() == ScreenGrabber::ActiveState) {
        m_screenGrabber->stop();

        while (m_screenGrabber->state() != ScreenGrabber::StoppedState) {}
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::on_MHStart_clicked()
{
    ui->MHStart->setEnabled(false);
    ui->MHStop->setEnabled(true);

    m_helper->startGrabbing();
    m_cursorTimer.start();
}

void MainWindow::on_MHStop_clicked()
{
    ui->MHStart->setEnabled(true);
    ui->MHStop->setEnabled(false);
    ui->MHStatusLabel->setText("-");

    m_helper->stopGrabbing();
    m_cursorTimer.stop();
}

void MainWindow::updateCursorLabel()
{
    ui->cursorLabel->setPixmap(MouseHelper::cursorPixmap());
}

void MainWindow::updateMHStatusLabel(const MouseEvent &event)
{
    QString str(tr("%1 button %2\nPosition: %3, %4"));
    str = str.arg(event.button == MouseEvent::LeftButton ? tr("Left") : tr("Right"));

    switch (event.type) {
    case MouseEvent::MouseButtonPress:
        str = str.arg(tr("pressed")).arg(event.position.x()).arg(event.position.x());
        break;

    case MouseEvent::MouseButtonRelease:
        str = str.arg(tr("released")).arg(event.position.x()).arg(event.position.x());
        break;

    case MouseEvent::MouseButtonClick:
        QPalette pal = ui->MHStatusLabel->palette();
        pal.setColor(ui->MHStatusLabel->backgroundRole(), QColor(Qt::red));
        ui->MHStatusLabel->setPalette(pal);

        QTimer::singleShot(150, this, SLOT(restoreLabelBackground()));
        return;
    }

    ui->MHStatusLabel->setText(str);
}

void MainWindow::restoreLabelBackground()
{
    ui->MHStatusLabel->setPalette(ui->MHStatus->palette());
}

void MainWindow::on_SGStart_clicked()
{
    m_screenGrabber->setCaptureRect(QRect(0, 0, ui->widthSpinBox->value(), ui->heightSpinBox->value()));
    m_screenGrabber->setLatency(ui->latencySpinBox->value());
    m_screenGrabber->start();

    ui->SGStart->setEnabled(false);
    ui->SGStop->setEnabled(true);
    ui->latencySpinBox->setEnabled(false);
    ui->widthSpinBox->setEnabled(false);
    ui->heightSpinBox->setEnabled(false);
}

void MainWindow::on_SGStop_clicked()
{
    m_screenGrabber->stop();

    ui->SGStop->setEnabled(false);
    ui->SGStart->setEnabled(true);
    ui->latencySpinBox->setEnabled(true);
    ui->widthSpinBox->setEnabled(true);
    ui->heightSpinBox->setEnabled(true);

    ui->SCFrameRateLabel->setText("-");

    m_grabbedFrameCount = 0;
    m_grabbingTime = 0;
}

void MainWindow::updateAverageFrameRate(const QImage &frame, int duration)
{
    Q_UNUSED(frame)

    if (m_screenGrabber->state() == ScreenGrabber::ActiveState) {
        ++m_grabbedFrameCount;
        m_grabbingTime += duration;

        if (m_grabbingTime != 0) {
            double frameRate = m_grabbedFrameCount / (m_grabbingTime / 1000.0);//frame per second
            ui->SCFrameRateLabel->setText(QString("%1 fps").arg(QString::number(frameRate, 'g', 4)));
        }
    }
}

void MainWindow::on_AGStart_clicked()
{
    m_audioGrabber->setDevice(QAudioDeviceInfo::defaultInputDevice());

    QAudioFormat format = QAudioDeviceInfo::defaultInputDevice().preferredFormat();
    format.setCodec("audio/pcm");
    format.setChannelCount(2);
    format.setSampleRate(8000);
    format.setSampleSize(16);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    m_audioGrabber->setFormat(format);

    //calc max input level amplitude
    switch (format.sampleSize()) {
    case 8:
        switch (format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 32767;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    if (!m_audioGrabber->start()) {
        qDebug()<<"Error: "<<m_audioGrabber->errorString();
    }
}

void MainWindow::on_AGStop_clicked()
{
    m_audioGrabber->stop();
}

void MainWindow::updateSoundPB(const QByteArray &data)
{
    if (m_maxAmplitude) {
        QAudioFormat format = m_audioGrabber->format();

        Q_ASSERT(format.sampleSize() % 8 == 0);
        const int channelBytes = format.sampleSize() / 8;
        const int sampleBytes = format.channels() * channelBytes;
        Q_ASSERT(data.size() % sampleBytes == 0);
        const int numSamples = data.size() / sampleBytes;

        int maxValue = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data.data());

        for (int i = 0; i < numSamples; ++i) {
            for(int j = 0; j < format.channels(); ++j) {
                int value = 0;

                if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } else if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::UnSignedInt) {
                    if (format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt) {
                    if (format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                }

                maxValue = qMax(value, maxValue);
                ptr += channelBytes;
            }
        }

        maxValue = qMin(maxValue, m_maxAmplitude);

        ui->SoundPB->setValue(qRound((qreal(maxValue) / m_maxAmplitude) * 100));
    }
}
