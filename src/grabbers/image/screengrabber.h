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

#ifndef SCREENGRABBER_H
#define SCREENGRABBER_H

#include "../../qtmel_global.h"
#include "abstractimagegrabber.h"
#include <QRect>
#include <QMutex>
#include "../../helpers/mousehelper.h"
#include <QElapsedTimer>

//! The ScreenGrabber class allows the application to capture frames from a desktop screen.
/*!
  This class is used to capture full or part screen images from the desktop. You can capture the screen with or without the cursor, use setCaptureCursor() for that.
  In order to capture part of the screen use setCaptureRect() function.

  ScreenGrabber will emit frameAvailable() whenever a new image will be available.

  Here is an example of ScreenGrabber usage:
  @code
  ScreenGrabber *grabber = new ScreenGrabber(this);
  grabber->setCaptureRect(QRect(0, 0, 640, 480));//capture the rectangle from (0, 0) point of the screen with the size 640x480
  grabber->setCaptureRect(false);//we don't want to see the cursor in the final video
  grabber->start();
  @endcode
*/
class QTMELSHARED_EXPORT ScreenGrabber : public AbstractImageGrabber
{
    Q_OBJECT

    /*!
      This property holds the capture rectangle.
      \sa setCaptureRect()
      \sa captureRect()
    */
    Q_PROPERTY(QRect captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)
    /*!
      This property holds whether to capture the cursor while grabbing.
      Default value is true.
      \sa setCaptureCursor()
      \sa isCaptureCursor()
    */
    Q_PROPERTY(bool isCaptureCursor READ isCaptureCursor WRITE setCaptureCursor NOTIFY isCaptureCursorChanged)
    Q_PROPERTY(bool isDrawClicks READ isDrawClicks WRITE setDrawClicks NOTIFY isDrawClicksChanged)
public:
    /*! Constructs a screen grabber with the given parent. */
    explicit ScreenGrabber(QObject *parent = 0);

    /*! Destroys the screen grabber. */
    ~ScreenGrabber();

    /*!
      Sets the capture rectangle. You can use it to capture part of the screen.
      \sa captureRect()
    */
    void setCaptureRect(const QRect &rect);
    /*!
      Returns current capture rectangle.
      \sa setCaptureRect()
    */
    QRect captureRect() const;

    void setCaptureCursor(bool capture); 
    bool isCaptureCursor() const;
    
    void setLeftClickFrames(const QStringList &frames);
    void setRightClickFrames(const QStringList &frames);

    void setDrawClicks(bool draw);
    bool isDrawClicks() const;
    
public Q_SLOTS:
    bool start();

Q_SIGNALS:
    void captureRectChanged(const QRect &rect);
    void isCaptureCursorChanged(bool capture);
    void isDrawClicksChanged(bool draw);
    
private Q_SLOTS:
    QImage currentCursor();
    QImage currentFrame();
    void onMousePress(const MouseEvent &event);
    void onStateChanged(AbstractGrabber::State state);

private:
    QImage captureFrame();
    void drawClick(QImage &frame);
    void initClickFrames();
    
    QRect m_captureRect;
    bool m_isCaptureCursor;
    bool m_isDrawClicks;

    QList<QPixmap> m_leftClickFramesList;
    QList<QPixmap> m_rightClickFramesList;
    QElapsedTimer m_leftClickTimer;
    QPoint m_leftClickPos;
    QElapsedTimer m_rightClickTimer;
    QPoint m_rightClickPos;

    MouseHelper* m_mouseHelper;

    mutable QMutex m_captureRectMutex;
    mutable QMutex m_captureCursorMutex;
    mutable QMutex m_drawClickMutex;
    void drawPixmapsOntoFrame(const QList<QPixmap> &listPixmaps, QImage &frame, QElapsedTimer &timer, const QPoint &clickPos, float fpsFrames = 0.01);
};

#endif // SCREENGRABBER_H
