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

#ifndef MOUSEHELPER_H
#define MOUSEHELPER_H

#include "../qtmel_global.h"
#include <QObject>
#include <QPoint>
#include <QPixmap>

//! The MouseEvent class contains parameters that describe a mouse event.
/*!
  Mouse events occur when a mouse button is pressed or released.
*/
class MouseEvent {
public:

    /*! This enum describes mouse buttons. */
    enum MouseButton {
        LeftButton = 1,/*!< The left button. */
        RightButton/*!< The right button. */
    };

    /*! This enum describes mouse event types. */
    enum Type {
        MouseButtonPress = 1,/*!< Mouse press. */
        MouseButtonRelease,/*!< Mouse release. */
        MouseButtonClick/*!< Mouse click. */
    };

    /*! Contains a type of an event. */
    MouseEvent::Type type;
    /*! Contains a mouse button of an event. */
    MouseEvent::MouseButton button;
    /*! Contains current cursor position. */
    QPoint position;
};

class MouseHelperPrivate;

//! The MouseHelper class provides the ability to catch global mouse events.
/*!
  This class helps you to handle global mouse events such as press, release and click.
  Also you can get the global cursor pixmap using static cursorPixmap() method.

  \sa MouseEvent
*/
class MouseHelper : public QObject
{
    Q_OBJECT
public:    
    explicit MouseHelper(QObject *parent = 0);
    ~MouseHelper();

    /*!
      Starts the global mouse events grabbing.
      \sa stopGrabbing()
    */
    void startGrabbing();
    /*!
      Stops the global mouse events grabbing.
      \sa startGrabbing()
    */
    void stopGrabbing();

    /*!
      Returns the global cursor pixmap.
    */
    static QPixmap cursorPixmap();

Q_SIGNALS:
    /*!
      This signal is emmited when new global mouse event occurs.
      \sa startGrabbing()
    */
    void mouseEvent(const MouseEvent &event);

private:
    MouseHelperPrivate *pimpl;
};

#endif // MOUSEHELPER_H
