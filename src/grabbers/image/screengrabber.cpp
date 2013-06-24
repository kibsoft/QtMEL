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
{
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

    return frame;
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
