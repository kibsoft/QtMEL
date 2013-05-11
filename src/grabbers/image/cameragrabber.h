#ifndef CAMERAGRABBER_H
#define CAMERAGRABBER_H

#include "../../qtmel_global.h"
#include "abstractimagegrabber.h"

class CvCapture;

class QTMELSHARED_EXPORT CameraGrabber : public AbstractImageGrabber
{
    Q_OBJECT
public:
    CameraGrabber(QObject *parent = 0);
    ~CameraGrabber();

    void setInitializationTime(int ms);
    int initializationTime() const;

    void setSize(const QSize &size);
    QSize size() const;

public Q_SLOTS:
    bool start();

private Q_SLOTS:
    void onStateChanged(AbstractGrabber::State state);

private:
    QImage captureFrame();

    CvCapture *m_camera;
    QSize m_size;
    int m_initTime;
};

#endif // CAMERAGRABBER_H
