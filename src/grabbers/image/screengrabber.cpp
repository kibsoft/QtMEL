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

#if QT_VERSION >= 0x050000
#include <QScreen>
#endif

#include "../../helpers/mousehelper.h"

ScreenGrabber::ScreenGrabber(QObject *parent)
    : AbstractImageGrabber(parent)
    , m_isCaptureCursor(true)
    , m_isDrawClicks(true)
    , m_mouseHelper(new MouseHelper(this))
{
    initClickFrames();

    connect(m_mouseHelper,SIGNAL(mouseEvent(MouseEvent)),SLOT(onMousePress(MouseEvent)));
    connect(this, SIGNAL(stateChanged(AbstractGrabber::State)), this, SLOT(onStateChanged(AbstractGrabber::State)));
}

void ScreenGrabber::initClickFrames()
{
    QStringList leftFramesPath;
    for(int i = 0 ;i < 5; ++i) {
        leftFramesPath << QString(":/resources/defaultLeftClickFrames/%1.png").arg(i);
    }
    setLeftClickFrames(leftFramesPath);

    QStringList rightFramesPath;
    for(int i = 0; i < 5; ++i) {
        rightFramesPath << QString(":/resources/defaultRightClickFrames/%1.png").arg(i);
    }
    setRightClickFrames(rightFramesPath);
}

ScreenGrabber::~ScreenGrabber()
{

}

void ScreenGrabber::setCaptureRect(const QRect &rect)
{
    QMutexLocker locker(&m_captureRectMutex);
    if (m_captureRect != rect) {
        m_captureRect = rect;

        Q_EMIT captureRectChanged(rect);
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
    if (m_isCaptureCursor != capture) {
        m_isCaptureCursor = capture;

        Q_EMIT isCaptureCursorChanged(capture);
    }
}

bool ScreenGrabber::isCaptureCursor() const
{
    QMutexLocker locker(&m_captureCursorMutex);
    return m_isCaptureCursor;
}

void ScreenGrabber::setLeftClickFrames(const QStringList &frames)
{
    if (state() == AbstractGrabber::StoppedState) {
        if (frames.count())
            m_leftClickFramesList.clear();

        Q_FOREACH (QString str, frames) {
            m_leftClickFramesList.append(QPixmap(str));
        }
    }
}

void ScreenGrabber::setRightClickFrames(const QStringList &frames)
{
    if (state() == AbstractGrabber::StoppedState) {
        if (frames.count())
            m_rightClickFramesList.clear();

        Q_FOREACH (QString str, frames) {
            m_rightClickFramesList.append(QPixmap(str));
        }
    }
}

void ScreenGrabber::setDrawClicks(bool draw)
{
    QMutexLocker locker(&m_drawClickMutex);
    if (m_isDrawClicks != draw) {
        m_isDrawClicks = draw;

        Q_EMIT isDrawClicksChanged(draw);
    }
}

bool ScreenGrabber::isDrawClicks() const
{
    QMutexLocker locker(&m_drawClickMutex);
    return m_isDrawClicks;
}

bool ScreenGrabber::start()
{
    //emit error signal if the capture rect is wrong
    if (captureRect().isNull() || !captureRect().isValid() || !qApp->desktop()->screen()->rect().contains(captureRect())
            || (captureRect().width() > qApp->desktop()->width() || captureRect().height() > qApp->desktop()->height())) {

        setError(AbstractGrabber::InvalidConfigurationError, tr("Capture rectangle is invalid"));
        return false;
    }

    return AbstractImageGrabber::start();
}

QImage ScreenGrabber::captureFrame()
{
    QImage frame;
    QMetaObject::invokeMethod(this, "currentFrame", Qt::BlockingQueuedConnection, Q_RETURN_ARG(QImage, frame));

    //draw cursor if needed
    if (isCaptureCursor()) {
        int rectLeft = captureRect().left();
        int rectTop = captureRect().top();
        int rectWidth = captureRect().width();
        int rectHeight = captureRect().height();

        int xDiff = QCursor::pos().x() - rectLeft;
        int yDiff = QCursor::pos().y() - rectTop;

        if (xDiff > 0 && xDiff < rectWidth
                && yDiff > 0 && yDiff < rectHeight) {
            QImage cursor;
            QMetaObject::invokeMethod(this, "currentCursor", Qt::BlockingQueuedConnection, Q_RETURN_ARG(QImage, cursor));
            if (!cursor.isNull()) {
                QPainter painter(&frame);
                painter.drawImage(QPoint(xDiff, yDiff), cursor);
            }
        }
    }

    if (isDrawClicks())
        drawClick(frame);
    
    return frame;
}

void ScreenGrabber::drawClick(QImage &frame)
{
    float fpsLeftClickFrames = 10;
    if (m_leftClickTimer.elapsed() != 0) {
        int timeAfterClick = m_leftClickTimer.elapsed();//ms
        int numFrame = (fpsLeftClickFrames/1000)*timeAfterClick;

        if(numFrame < m_leftClickFramesList.size()) {
            QPixmap pixMap = m_leftClickFramesList.at(numFrame);
            QPainter painter(&frame);
            int xCoord = m_leftClickPos.x() - pixMap.width()/2 - captureRect().left();
            int yCoord = m_leftClickPos.y() - pixMap.height()/2 - captureRect().top();

            painter.drawPixmap(xCoord, yCoord, pixMap);
        } else {
            m_leftClickTimer = QTime();
        }
    }

    float fpsRightClickFrames = 10;
    if (m_rightClickTimer.elapsed() != 0) {
        int timeAfterClick = m_rightClickTimer.elapsed();//ms
        int numFrame = (fpsRightClickFrames/1000) * timeAfterClick;

        if(numFrame < m_rightClickFramesList.size()) {
            QPixmap pixMap = m_rightClickFramesList.at(numFrame);
            QPainter painter(&frame);
            int xCoord = m_rightClickPos.x() - pixMap.width()/2 - captureRect().left();
            int yCoord = m_rightClickPos.y() - pixMap.height()/2 - captureRect().top();

            painter.drawPixmap(xCoord, yCoord, pixMap);
        } else {
            m_rightClickTimer = QTime();
        }
    }
}

QImage ScreenGrabber::currentCursor()
{
    return MouseHelper::cursorPixmap().toImage();
}

QImage ScreenGrabber::currentFrame()
{
    QPixmap pixmap;
#if QT_VERSION >= 0x050000
    pixmap = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), captureRect().left(), captureRect().top(),
                                               captureRect().width(), captureRect().height());
#else
    pixmap = QPixmap::grabWindow(qApp->desktop()->winId(), captureRect().left(), captureRect().top(),
                                 captureRect().width(), captureRect().height());
#endif

    return pixmap.toImage();
}

void ScreenGrabber::onMousePress(const MouseEvent &event)
{
    if(state() == AbstractGrabber::ActiveState) {
        if(isDrawClicks()) {
            if(event.type == MouseEvent::MouseButtonPress) {
                if(event.button == MouseEvent::LeftButton) {
                    m_leftClickTimer = QTime();
                    m_leftClickTimer.start();
                    m_leftClickPos = event.position;
                } else {
                    m_rightClickTimer = QTime();
                    m_rightClickTimer.start();
                    m_rightClickPos = event.position;
                }
            }
        }
    }
}

void ScreenGrabber::onStateChanged(AbstractGrabber::State state)
{
    if (state == AbstractGrabber::ActiveState)
        m_mouseHelper->startGrabbing();
    else if (state == AbstractGrabber::StoppedState)
        m_mouseHelper->stopGrabbing();
}
