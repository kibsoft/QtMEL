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

//! The CameraGrabber class allows the application to capture frames from a camera device.
/*!
  Using this class you can capture frames with variable frame rate from a camera device. In order to get available devices
  call the availableDeviceNames() static function. To let CameraGrabber know which camera it must grab pass the device index to the setDeviceIndex() function.
  The CameraGrabber class has the setInitializationTime() function to set time the app will wait before grabbing start. It is need to be used because you
  will get black(or other colors) frames while a device is turning on. To get maximum frame size supported by a device use the the maximumFrameSize() static function.
  If you want to get a frame with the size that smaller or larger than maximumFrameSize() then you can set the needed size calling the setSize() function.

  The signal frameAvailable() is emmited whenever a new frame is available.
*/
class QTMELSHARED_EXPORT CameraGrabber : public AbstractImageGrabber
{
    Q_OBJECT
public:
    CameraGrabber(QObject *parent = 0);
    ~CameraGrabber();

    /*!
      Lets CameraGrabber know which device it must grab.
      \sa deviceIndex()
      \sa availableDeviceNames()
    */
    void setDeviceIndex(int index);
    /*!
      Returns current device index.
      \sa setDeviceIndex()
    */
    int deviceIndex() const;

    /*!
      Sets device initialization time in milliseconds.
      Default value is 1000ms.
      \sa initializationTime()
    */
    void setInitializationTime(int ms);
    /*!
      Returns device initialization time in milliseconds.
      \sa setInitializationTime()
    */
    int initializationTime() const;

    /*!
      Sets the frame size. A frame will be scaled as large as possible inside a given size, preserving the aspect ratio.
      \sa size()
    */
    void setSize(const QSize &size);
    /*!
      Returns the frame size.
      \sa setSize()
    */
    QSize size() const;

    /*!
      Returns names of available cameras.
    */
    static QStringList availableDeviceNames();
    /*!
      Returns the maximum frame size supported by a device.
      \sa availableDeviceNames()
    */
    static QSize maximumFrameSize(int deviceIndex);

public Q_SLOTS:
    bool start();

Q_SIGNALS:
    /*!
      The signal is emmited when initialization time is out.
      \sa setInitializationTime()
    */
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
