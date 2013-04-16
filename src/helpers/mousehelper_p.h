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

#ifndef MOUSEHELPER_P_H
#define MOUSEHELPER_P_H

#include <QObject>
#include "mousehelper.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif


class MouseHelperPrivate : public QObject {
    Q_OBJECT

public:
    explicit MouseHelperPrivate(QObject *parent = 0);
    ~MouseHelperPrivate();

    void startGrabbing();
    void stopGrabbing();

    static QPixmap cursorPixmap();

Q_SIGNALS:
    void mouseEvent(const MouseEvent &event);

private Q_SLOTS:
    void handleEvent(const MouseEvent &event);

private:
    void sendClickEvent(MouseEvent::MouseButton button, const QPoint &position);

#ifdef Q_OS_WIN
    //hook object that is returned after SetWindowsHookEx
    HHOOK m_hook;
#endif

    bool m_leftButtonDown;
    bool m_rightButtonDown;
};


#endif // MOUSEHELPER_P_H
