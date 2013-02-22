#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_helper(new MouseHelper(this))
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowStaysOnTopHint);

    connect(m_helper, SIGNAL(mouseEvent(MouseEvent)), this, SLOT(updateMHStatusLabel(MouseEvent)));
}

MainWindow::~MainWindow()
{
    delete ui;
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
    Q_UNUSED(event)

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
