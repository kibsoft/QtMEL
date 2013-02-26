#include "screengrabber.h"
#include <QElapsedTimer>
#include <QImage>
#include <QEventLoop>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QMutexLocker>

#include "../../helpers/mousehelper.h"

ScreenGrabber::ScreenGrabber(QObject *parent)
    : AbstractImageGrabber(parent)
    , m_captureCursor(true)
{
}

void ScreenGrabber::setCaptureRect(const QRect &rect)
{
    QMutexLocker locker(&m_captureRectMutex);
    if (m_captureRect != rect) {
        m_captureRect = rect;

        emit captureRectChanged();
    }
}

QRect ScreenGrabber::captureRect() const
{
    QMutexLocker locker(&m_captureRectMutex);
    return m_captureRect;
}

void ScreenGrabber::setCaptureCursor(bool capture)
{
    QMutexLocker locker(&m_captureCursorMutex);
    if (m_captureCursor != capture) {
        m_captureCursor = capture;

        emit captureCursorChanged();
    }
}

bool ScreenGrabber::captureCursor() const
{
    QMutexLocker locker(&m_captureCursorMutex);
    return m_captureCursor;
}

bool ScreenGrabber::start()
{
    //emit error signal if the capture rect is wrong
    if (captureRect().isNull() || !captureRect().isValid()
            || (captureRect().width() > qApp->desktop()->width() || captureRect().height() > qApp->desktop()->height())) {

        setError(AbstractGrabber::InvalidConfigurationError, tr("Capture rectangle is invalid"));
        return false;
    }

    return AbstractImageGrabber::start();
}

void ScreenGrabber::grab()
{
    QElapsedTimer durationTimer;
    durationTimer.start();

    int rectLeft = captureRect().left();
    int rectTop = captureRect().top();
    int rectWidth = captureRect().width();
    int rectHeight = captureRect().height();

    QImage frame;//this stores grabbed image
    QEventLoop latencyLoop;

    forever {
        //check if we must finish grabbing
        if (stopRequest() || pauseRequest())
            break;

        //restart the timer in order to get right duration of the image capture
        durationTimer.restart();

        frame = QPixmap::grabWindow(qApp->desktop()->winId(), rectLeft, rectTop,
                                    rectWidth, rectHeight).toImage();//convert to QImage because we can't use QPixmap in the thread other than GUI

        //draw cursor if needed
        if (captureCursor()) {
            int xDiff = QCursor::pos().x() - rectLeft;
            int yDiff = QCursor::pos().y() - rectTop;

            if (xDiff > 0 && xDiff < rectWidth
                    && yDiff > 0 && yDiff < rectHeight) {
                QPainter painter(&frame);
                painter.drawImage(QCursor::pos(), MouseHelper::cursorPixmap().toImage());
            }
        }

        //wait for set by user milliseconds
        QTimer::singleShot(latency(), &latencyLoop, SLOT(quit()));
        latencyLoop.exec();

        emit frameAvailable(frame, durationTimer.elapsed());
    }

    setState(stopRequest() ? AbstractGrabber::StoppedState : AbstractGrabber::SuspendedState);

    //reset stop and pause flags
    setStopRequest(false);
    setPauseRequest(false);
}
