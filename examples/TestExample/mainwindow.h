#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MouseHelper>

namespace Ui {
class MainWindow;
}

class ScreenGrabber;
class AudioGrabber;

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

    void on_AGStart_clicked();
    void on_AGStop_clicked();
    void updateSoundPB(const QByteArray &data);
private:
    Ui::MainWindow *ui;
    MouseHelper *m_helper;
    ScreenGrabber *m_screenGrabber;

    AudioGrabber *m_audioGrabber;
    int m_maxAmplitude;

    int m_grabbingTime;
    int m_grabbedFrameCount;
};

#endif // MAINWINDOW_H
