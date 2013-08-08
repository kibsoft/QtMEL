/****************************************************************************
**
** QtMEL is a Qt Media Encoding Library that allows to encode video and audio streams
** Copyright (C) 2013 Kirill Bukaev(aka KIBSOFT).
** Contact: Kirill Bukaev (support@kibsoft.ru)
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/

#include "abstractimagegrabber.h"
#include "../../helpers/audiotimer.h"

#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif

#include <QMutexLocker>
#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>
#include <QPixmap>
#include <QPainter>

AbstractImageGrabber::AbstractImageGrabber(QObject *parent)
    : AbstractGrabber(parent)
    , m_prevPts(-1)
    , m_latency(0)
    , m_grabbedFrameCount(0)
    , m_isStopRequest(false)
    , m_isPauseRequest(false)
    , m_timer(0)
    , m_initTime(0)
{
}

AbstractImageGrabber::~AbstractImageGrabber()
{
}

void AbstractImageGrabber::setLatency(int latency)
{
    QMutexLocker locker(&m_latencyMutex);
    if (m_latency != latency) {
        m_latency = latency;

        Q_EMIT latencyChanged(latency);
    }
}

int AbstractImageGrabber::latency() const
{
    QMutexLocker locker(&m_latencyMutex);
    return m_latency;
}

void AbstractImageGrabber::setInitializationTime(int ms)
{
    if (m_initTime != ms) {
        m_initTime = ms;
    }
}

int AbstractImageGrabber::initializationTime() const
{
    return m_initTime;
}

int AbstractImageGrabber::grabbedFrameCount() const
{
    QMutexLocker locker(&m_grabbedFrameCountMutex);
    return m_grabbedFrameCount;
}

bool AbstractImageGrabber::start()
{
    if (state() == AbstractGrabber::StoppedState) {
        waitForInitialization();

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
        setPauseRequest(true);
    }
}

void AbstractImageGrabber::resume()
{
    if (state() == AbstractGrabber::SuspendedState)
        startGrabbing();
}

void AbstractImageGrabber::startGrabbing()
{
    //init grabbed frames
    m_grabbedFrameCount = 0;

    QtConcurrent::run(this, &AbstractImageGrabber::grab);
    setState(AbstractGrabber::ActiveState);
}

void AbstractImageGrabber::grab()
{
    QImage frame;//this stores grabbed image
    QEventLoop latencyLoop;
    QElapsedTimer timer;

    if (!m_timer) {
        timer.start();
    }

    m_prevPts = -1;
    int pts = -1;

    Q_FOREVER {
        frame = captureFrame();
        
        setGrabbedFrameCount(grabbedFrameCount() + 1);
        
        pts = m_timer ? m_timer->elapsed() : timer.elapsed();
        if (m_prevPts != pts) {
            m_prevPts = pts;
            Q_EMIT frameAvailable(frame, pts);
        }

        //check if we must finish grabbing
        if (isStopRequest() || isPauseRequest())
            break;

        //wait for set by user milliseconds
        QTimer::singleShot(latency(), &latencyLoop, SLOT(quit()));
        latencyLoop.exec();
    }

    setState(isStopRequest() ? AbstractGrabber::StoppedState : AbstractGrabber::SuspendedState);

    if (isStopRequest())
        m_prevPts = -1;

    //reset stop and pause flags
    setStopRequest(false);
    setPauseRequest(false);
}

void AbstractImageGrabber::setGrabbedFrameCount(int count)
{
    QMutexLocker locker(&m_grabbedFrameCountMutex);
    if (m_grabbedFrameCount != count) {
        m_grabbedFrameCount = count;

        Q_EMIT grabbedFrameCountChanged(count);
    }
}

void AbstractImageGrabber::setStopRequest(bool stop)
{
    QMutexLocker locker(&m_stopPauseMutex);

    if (m_isStopRequest != stop) {
        m_isStopRequest = stop;
    }
}

bool AbstractImageGrabber::isStopRequest() const
{
    QMutexLocker locker(&m_stopPauseMutex);

    return m_isStopRequest;
}

void AbstractImageGrabber::setPauseRequest(bool pause)
{
    QMutexLocker locker(&m_stopPauseMutex);

    if (m_isPauseRequest != pause) {
        m_isPauseRequest = pause;
    }
}

bool AbstractImageGrabber::isPauseRequest() const
{
    QMutexLocker locker(&m_stopPauseMutex);

    return m_isPauseRequest;
}

void AbstractImageGrabber::waitForInitialization()
{
    QEventLoop initLoop;
    QTimer::singleShot(m_initTime, &initLoop, SLOT(quit()));
    initLoop.exec();

    Q_EMIT initialized();
}

void AbstractImageGrabber::setTimer(AudioTimer *timer)
{
    m_timer = timer;
}
