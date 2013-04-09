#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QTimer>
#include <ScreenGrabber>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_screenGrabber(new ScreenGrabber(this))
  , m_helper(new MouseHelper(this))
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

    //connect signals
    connect(m_helper, SIGNAL(mouseEvent(MouseEvent)), this, SLOT(updateMHStatusLabel(MouseEvent)));
    connect(m_screenGrabber, SIGNAL(frameAvailable(QImage,int)), this, SLOT(updateAverageFrameRate(QImage,int)));
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
}

void MainWindow::on_MHStop_clicked()
{
    ui->MHStart->setEnabled(true);
    ui->MHStop->setEnabled(false);
    ui->MHStatusLabel->setText("-");

    m_helper->stopGrabbing();
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
