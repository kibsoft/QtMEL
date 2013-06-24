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

#include "mousehelper_p.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xfixes.h>

MouseHelperPrivate::MouseHelperPrivate(QObject *parent)
    : QObject(parent)
    , m_leftButtonDown(false)
    , m_rightButtonDown(false)
{
}

MouseHelperPrivate::~MouseHelperPrivate()
{
    stopGrabbing();
}

void MouseHelperPrivate::startGrabbing()
{
}

void MouseHelperPrivate::stopGrabbing()
{
}

void MouseHelperPrivate::handleEvent(const MouseEvent &event)
{
    Q_UNUSED(event)
}

void MouseHelperPrivate::sendClickEvent(MouseEvent::MouseButton button, const QPoint &position)
{
    Q_UNUSED(button)
    Q_UNUSED(position)
}

QPixmap MouseHelperPrivate::cursorPixmap()
{
    XFixesCursorImage *xfCursorImage = XFixesGetCursorImage(XOpenDisplay(NULL));
    QImage cursorImage((uchar*)xfCursorImage->pixels, xfCursorImage->width, xfCursorImage->height, QImage::Format_ARGB32_Premultiplied);

    return QPixmap::fromImage(cursorImage);
}
