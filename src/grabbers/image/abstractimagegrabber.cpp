#include "abstractimagegrabber.h"
#include <QtConcurrentRun>
#include <QMutexLocker>

AbstractImageGrabber::AbstractImageGrabber(QObject *parent)
    : AbstractGrabber(parent)
    , m_latency(0)
    , m_stopRequest(false)
    , m_pauseRequest(false)
{
}

AbstractImageGrabber::~AbstractImageGrabber()
{
}

void AbstractImageGrabber::setLatency(int ms)
{
    if (m_latency != ms) {
        m_latency = ms;

        emit latencyChanged();
    }
}

int AbstractImageGrabber::latency() const
{
    return m_latency;
}

bool AbstractImageGrabber::start()
{
    if (state() == AbstractGrabber::StoppedState) {
        startGrabbing();
        return true;
    }

    return false;
}

void AbstractImageGrabber::stop()
{
    if (state() != AbstractGrabber::StoppedState) {
        setStopRequest(true);
    }
}

void AbstractImageGrabber::suspend()
{
    if (state() == AbstractGrabber::ActiveState) {
        QMutexLocker locker(&m_stopPauseMutex);
        m_pauseRequest = true;
    }
}

void AbstractImageGrabber::resume()
{
    if (state() == AbstractGrabber::SuspendedState)
        startGrabbing();
}

void AbstractImageGrabber::startGrabbing()
{
    QtConcurrent::run(this, &AbstractImageGrabber::grab);
    setState(AbstractGrabber::ActiveState);
}

void AbstractImageGrabber::setStopRequest(bool stop)
{
    QMutexLocker locker(&m_stopPauseMutex);

    if (m_stopRequest != stop) {
        m_stopRequest = stop;
    }
}

bool AbstractImageGrabber::stopRequest() const
{
    QMutexLocker locker(&m_stopPauseMutex);

    return m_stopRequest;
}

void AbstractImageGrabber::setPauseRequest(bool pause)
{
    QMutexLocker locker(&m_stopPauseMutex);

    if (m_pauseRequest != pause) {
        m_pauseRequest = pause;
    }
}

bool AbstractImageGrabber::pauseRequest() const
{
    QMutexLocker locker(&m_stopPauseMutex);

    return m_pauseRequest;
}
