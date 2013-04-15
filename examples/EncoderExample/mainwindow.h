#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Encoder>
#include <ScreenGrabber>
#include <VideoCodecSettings>
#include <AudioCodecSettings>

namespace Ui {
class MainWindow;
}

class ScreenGrabber;
class AudioGrabber;
class Encoder;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void onEncoderError(Encoder::Error error);
    void onImageGrabberError(AbstractGrabber::Error error);
    void onAudioGrabberError(AbstractGrabber::Error error);
    void onState(Encoder::State state);

    void on_chooseFileButton_clicked();

    void on_StartButton_clicked();

    void on_StopButton_clicked();

    void on_fixedFPSStart_clicked();

private:
    void generateFrames();
    void showMessage(const QString &title, const QString &message);
    VideoCodecSettings videoCodecSettings() const;
    AudioCodecSettings audioCodecSettings() const;

    Ui::MainWindow *ui;
    ScreenGrabber *m_screenGrabber;
    AudioGrabber *m_audioGrabber;
    Encoder *m_encoder;
};

#endif // MAINWINDOW_H
