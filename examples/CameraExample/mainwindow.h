#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <VideoCodecSettings>
#include <AudioCodecSettings>
#include <Encoder>
#include <AbstractGrabber>

namespace Ui {
class MainWindow;
}

class Recorder;
class CameraGrabber;
class AudioGrabber;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();

    void on_startButton_clicked();

    void on_pauseButton_clicked();

    void on_resumeButton_clicked();

    void on_stopButton_clicked();

    void showFrame(const QImage &frame);

    void onEncoderError(Encoder::Error error);
    void onGrabberError(AbstractGrabber::Error error);

private:
    void initFrameLabel();

    VideoCodecSettings videoCodecSettings() const;
    AudioCodecSettings audioCodecSettings() const;

    Ui::MainWindow *ui;
    Recorder *m_recorder;
    CameraGrabber *m_cameraGrabber;
    AudioGrabber *m_audioGrabber;
};

#endif // MAINWINDOW_H
