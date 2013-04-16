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

#include "abstractgrabber.h"


AbstractGrabber::AbstractGrabber(QObject *parent)
    : QObject(parent)
    , m_state(AbstractGrabber::StoppedState)
    , m_error(AbstractGrabber::NoError)
{
}

AbstractGrabber::~AbstractGrabber()
{
}

AbstractGrabber::State AbstractGrabber::state() const
{
    return m_state;
}

AbstractGrabber::Error AbstractGrabber::error() const
{
    return m_error;
}

QString AbstractGrabber::errorString() const
{
    return m_errorString;
}

void AbstractGrabber::setState(AbstractGrabber::State state)
{
    if (m_state != state) {
        m_state = state;

        Q_EMIT stateChanged();
    }
}

void AbstractGrabber::setError(AbstractGrabber::Error errorCode, const QString &errorString)
{
    m_error = errorCode;
    m_errorString = errorString;

    Q_EMIT error(errorCode);
}
