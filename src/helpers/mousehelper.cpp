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

#include "mousehelper.h"
#include "mousehelper_p.h"

MouseHelper::MouseHelper(QObject *parent) :
    QObject(parent)
  , pimpl(new MouseHelperPrivate(this))
{
    connect(pimpl, SIGNAL(mouseEvent(MouseEvent)), this, SIGNAL(mouseEvent(MouseEvent)));
}

MouseHelper::~MouseHelper()
{
}

void MouseHelper::startGrabbing()
{
    pimpl->startGrabbing();
}

void MouseHelper::stopGrabbing()
{
    pimpl->stopGrabbing();
}

QPixmap MouseHelper::cursorPixmap()
{
    return MouseHelperPrivate::cursorPixmap();
}
