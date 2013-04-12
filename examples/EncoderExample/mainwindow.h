#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Encoder>
#include <ScreenGrabber>
#include <VideoCodecSettings>

namespace Ui {
class MainWindow;
}

class ScreenGrabber;
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
    void onState(Encoder::State state);

    void on_chooseFileButton_clicked();

    void on_StartButton_clicked();

    void on_StopButton_clicked();

    void on_fixedFPSStart_clicked();

private:
    void generateFrames();
    void showMessage(const QString &title, const QString &message);
    VideoCodecSettings videoCodecSettings() const;

    Ui::MainWindow *ui;
    ScreenGrabber *m_screenGrabber;
    Encoder *m_encoder;
};

#endif // MAINWINDOW_H
