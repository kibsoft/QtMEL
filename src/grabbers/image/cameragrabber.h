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

#ifndef CAMERAGRABBER_H
#define CAMERAGRABBER_H

#include "../../qtmel_global.h"
#include "abstractimagegrabber.h"

#include <QStringList>

class CvCapture;

class QTMELSHARED_EXPORT CameraGrabber : public AbstractImageGrabber
{
    Q_OBJECT
public:
    CameraGrabber(QObject *parent = 0);
    ~CameraGrabber();

    void setDeviceIndex(int index);
    int deviceIndex() const;

    void setInitializationTime(int ms);
    int initializationTime() const;

    void setSize(const QSize &size);
    QSize size() const;

    static QStringList availableDeviceNames();

public Q_SLOTS:
    bool start();

Q_SIGNALS:
    void initialized();

private Q_SLOTS:
    void onStateChanged(AbstractGrabber::State state);

private:
    bool createCamera();
    void releaseCamera();
    QImage captureFrame();

    CvCapture *m_camera;
    int m_deviceIndex;
    QSize m_size;
    int m_initTime;
};

#endif // CAMERAGRABBER_H
