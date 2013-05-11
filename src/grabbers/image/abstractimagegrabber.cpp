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
#include <QtConcurrentRun>
#include <QMutexLocker>
#include <QEventLoop>
#include <QTimer>

#ifdef Q_WS_WIN
#include <windows.h>
#else
#include <QTime>
#endif

AbstractImageGrabber::AbstractImageGrabber(QObject *parent)
    : AbstractGrabber(parent)
    , m_latency(0)
    , m_grabbedFrameCount(0)
    , m_isStopRequest(false)
    , m_isPauseRequest(false)
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

int AbstractImageGrabber::grabbedFrameCount() const
{
    QMutexLocker locker(&m_grabbedFrameCountMutex);
    return m_grabbedFrameCount;
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
        m_isPauseRequest = true;
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
#ifdef Q_WS_WIN
    int start = 0;
#else
    QTime time;
#endif
    Q_FOREVER {
#ifdef Q_WS_WIN
        start = GetTickCount();
#else
        time.start();
#endif

        //check if we must finish grabbing
        if (isStopRequest() || isPauseRequest())
            break;

        frame = captureFrame();

        //wait for set by user milliseconds
        QTimer::singleShot(latency(), &latencyLoop, SLOT(quit()));
        latencyLoop.exec();

        setGrabbedFrameCount(grabbedFrameCount() + 1);

#ifdef Q_WS_WIN
        Q_EMIT frameAvailable(frame, GetTickCount() - start);
#else
        Q_EMIT frameAvailable(frame, time.restart());
#endif
    }

    setState(isStopRequest() ? AbstractGrabber::StoppedState : AbstractGrabber::SuspendedState);

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
