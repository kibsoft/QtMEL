#include "abstractimagegrabber.h"
#include <QtConcurrentRun>
#include <QMutexLocker>

AbstractImageGrabber::AbstractImageGrabber(QObject *parent)
    : AbstractGrabber(parent)
    , m_stopRequested(false)
    , m_pauseRequested(false)
{
}

AbstractImageGrabber::~AbstractImageGrabber()
{
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
        QMutexLocker locker(&m_mutex);
        m_stopRequested = true;
    }
}

void AbstractImageGrabber::suspend()
{
    if (state() == AbstractGrabber::ActiveState) {
        QMutexLocker locker(&m_mutex);
        m_pauseRequested = true;
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
