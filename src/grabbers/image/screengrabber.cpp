#include "screengrabber.h"
#include <QElapsedTimer>
#include <QImage>
#include <QEventLoop>
#include <QPixmap>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>

ScreenGrabber::ScreenGrabber(QObject *parent)
    : AbstractImageGrabber(parent)
{
}

void ScreenGrabber::setCaptureRect(const QRect &rect)
{
    if (m_captureRect != rect) {
        m_captureRect = rect;

        emit captureRectChanged();
    }
}

QRect ScreenGrabber::captureRect() const
{
    return m_captureRect;
}

bool ScreenGrabber::start()
{
    //emit error signal if the capture rect is wrong
    if (m_captureRect.isNull() || !m_captureRect.isValid()
            || (m_captureRect.width() > qApp->desktop()->width() || m_captureRect.height() > qApp->desktop()->height())) {

        setError(AbstractGrabber::InvalidConfigurationError, tr("Capture rectangle is invalid"));
        return false;
    }

    return AbstractImageGrabber::start();
}

void ScreenGrabber::grab()
{
    QElapsedTimer durationTimer;
    durationTimer.start();

    int rectLeft = m_captureRect.left();
    int rectTop = m_captureRect.top();
    int rectWidth = m_captureRect.width();
    int rectHeight = m_captureRect.height();

    QImage frame;//this stores grabbed image
    QEventLoop latencyLoop;

    forever {
        m_mutex.lock();
        //check if we must finish grabbing
        if (m_stopRequested || m_pauseRequested)
            break;
        m_mutex.unlock();

        //restart the timer in order to get right duration of the image capture
        durationTimer.restart();

        frame = QPixmap::grabWindow(qApp->desktop()->winId(), rectLeft, rectTop,
                                    rectWidth, rectHeight).toImage();//convert to QImage because we can't use QPixmap in the thread other than GUI

        //wait for set by user milliseconds
        QTimer::singleShot(latency(), &latencyLoop, SLOT(quit()));
        latencyLoop.exec();

        emit frameAvailable(frame, durationTimer.elapsed());
    }

    setState(m_stopRequested ? AbstractGrabber::StoppedState : AbstractGrabber::SuspendedState);

    //reset stop and pause flags
    m_stopRequested = false;
    m_pauseRequested = false;
}
