#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MouseHelper>
#include <QRunnable>
#include <QApplication>

namespace Ui {
class MainWindow;
}

class ScreenGrabber;

class HelloWorldTask : public QRunnable
{
public:
    HelloWorldTask(QImage frame, int number)
    {
        m_frame = frame;
        m_number = number;
    }

    void run()
    {
        m_frame.save(QApplication::applicationDirPath() + "/" + QString::number(m_number) + ".jpg");
    }

private:
    QImage m_frame;
    int m_number;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_MHStart_clicked();
    void on_MHStop_clicked();

    void updateMHStatusLabel(const MouseEvent &event);
    void restoreLabelBackground();

    void on_SGStart_clicked();
    void on_SGStop_clicked();
    void updateAverageFrameRate(const QImage &frame, int duration);

private:
    Ui::MainWindow *ui;
    MouseHelper *m_helper;
    ScreenGrabber *m_screenGrabber;

    int m_grabbingTime;
    int m_grabbedFrameCount;
};

#endif // MAINWINDOW_H
