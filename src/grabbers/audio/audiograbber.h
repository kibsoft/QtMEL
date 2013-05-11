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

#ifndef AUDIOGRABBER_H
#define AUDIOGRABBER_H

#include "../../qtmel_global.h"
#include "../abstractgrabber.h"

#include <QAudioFormat>
#include <QPointer>

class QAudioInput;

class QTMELSHARED_EXPORT AudioGrabber : public AbstractGrabber
{
    Q_OBJECT

public:
    explicit AudioGrabber(QObject *parent = 0);
    ~AudioGrabber();

    void setDeviceIndex(int index);
    int deviceIndex() const;

    void setFormat(const QAudioFormat &format);
    QAudioFormat format() const;

    int grabbedAudioDataSize() const;

    static QStringList availableDeviceNames();

public Q_SLOTS:
    bool start();
    void stop();
    void suspend();
    void resume();

Q_SIGNALS:
    void frameAvailable(const QByteArray &frame);

private Q_SLOTS:
    void onReadyRead();

private:
    void init();
    void cleanup();

    QPointer<QAudioInput> m_inputDevice;
    QAudioFormat m_format;
    QIODevice *m_buffer;
    int m_deviceIndex;
    int m_grabbedAudioDataSize;
};

#endif // AUDIOGRABBER_H
