#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MouseHelper>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_MHStart_clicked();
    void on_MHStop_clicked();

    void updateMHStatusLabel(const MouseEvent &event);
    void restoreLabelBackground();

private:
    Ui::MainWindow *ui;
    MouseHelper *m_helper;
};

#endif // MAINWINDOW_H
