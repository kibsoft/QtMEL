#include "cameragrabber.h"

#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>
#include <QDateTime>
#include <opencv/cv.h>
#include <opencv/highgui.h>

CameraGrabber::CameraGrabber(QObject *parent)
    : AbstractImageGrabber(parent)
    , m_initTime(1000)
{
    connect(this, SIGNAL(stateChanged(AbstractGrabber::State)), this, SLOT(onStateChanged(AbstractGrabber::State)));
}

CameraGrabber::~CameraGrabber()
{
}

void CameraGrabber::setInitializationTime(int ms)
{
    if (m_initTime != ms) {
        m_initTime = ms;
    }
}

int CameraGrabber::initializationTime() const
{
    return m_initTime;
}

void CameraGrabber::setSize(const QSize &size)
{
}

QSize CameraGrabber::size() const
{
    return m_size;
}

bool CameraGrabber::start()
{
    m_camera = cvCreateCameraCapture(0);

    QEventLoop initLoop;
    QTimer::singleShot(m_initTime, &initLoop, SLOT(quit()));
    initLoop.exec();

    return AbstractImageGrabber::start();
}

void CameraGrabber::onStateChanged(AbstractGrabber::State state)
{
    if (state == AbstractGrabber::StoppedState
            || state == AbstractGrabber::SuspendedState) {
        cvReleaseCapture(&m_camera);
        m_camera = 0;
    }
}

QImage CameraGrabber::captureFrame()
{
    IplImage *iplImage = cvQueryFrame(m_camera);
    int height = iplImage->height;
    int width = iplImage->width;

    QImage frame;
    if (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3) {
        uchar *data = (uchar*)iplImage->imageData;
        frame = QImage(data, width, height, QImage::Format_RGB888).rgbSwapped();
    }

    return frame;

}
