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

#include "cameragrabber.h"

#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>
#include <QDateTime>
#include <QPainter>

#include <QCamera>

#include <opencv/cv.h>
#include <opencv/highgui.h>

CameraGrabber::CameraGrabber(QObject *parent)
    : AbstractImageGrabber(parent)
    , m_deviceIndex(-1)
{
    setInitializationTime(1000);

    connect(this, SIGNAL(stateChanged(AbstractGrabber::State)), this, SLOT(onStateChanged(AbstractGrabber::State)));
}

CameraGrabber::~CameraGrabber()
{
}

void CameraGrabber::setDeviceIndex(int index)
{
    if (m_deviceIndex != index) {
        m_deviceIndex = index;
    }
}

int CameraGrabber::deviceIndex() const
{
    return m_deviceIndex;
}

void CameraGrabber::setSize(const QSize &size)
{
    if (m_size != size) {
        m_size = size;
    }
}

QSize CameraGrabber::size() const
{
    return m_size;
}

QStringList CameraGrabber::availableDeviceNames()
{
    QByteArray device;
    QList<QByteArray> devices = QCamera::availableDevices();
    QStringList names;

    Q_FOREACH (device, devices) {
        names.append(QCamera::deviceDescription(device));
    }

    return names;
}

QSize CameraGrabber::maximumFrameSize(int deviceIndex)
{
    if (deviceIndex < 0 || deviceIndex > CameraGrabber::availableDeviceNames().count())
        return QSize();

    CvCapture *capture = cvCreateCameraCapture(deviceIndex);
    QSize size;
    size.setWidth(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH));
    size.setHeight(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
    cvReleaseCapture(&capture);

    return size;
}

bool CameraGrabber::start()
{
    if (!createCamera())
        return false;

    return AbstractImageGrabber::start();
}

void CameraGrabber::onStateChanged(AbstractGrabber::State state)
{
    if (state == AbstractGrabber::StoppedState) {
        releaseCamera();
    }
}

bool CameraGrabber::createCamera()
{
    if (m_deviceIndex < 0 || m_deviceIndex > CameraGrabber::availableDeviceNames().count()) {
        setError(AbstractGrabber::DeviceNotFoundError, tr("Device to be grabbed was not found."));
        return false;
    }
    m_camera = cvCreateCameraCapture(m_deviceIndex);

    //init size
    if (!size().isValid()) {
        QSize size;
        size.setWidth(cvGetCaptureProperty(m_camera, CV_CAP_PROP_FRAME_WIDTH));
        size.setHeight(cvGetCaptureProperty(m_camera, CV_CAP_PROP_FRAME_HEIGHT));
        setSize(size);
    }

    return true;
}

void CameraGrabber::releaseCamera()
{
    cvReleaseCapture(&m_camera);
    m_camera = 0;
}

QImage CameraGrabber::captureFrame()
{
    IplImage *iplImage = cvQueryFrame(m_camera);
    int height = iplImage->height;
    int width = iplImage->width;

    QImage frame;
    if (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3) {
        uchar *data = (uchar*)iplImage->imageData;
        frame = QImage(data, width, height, QImage::Format_RGB888).rgbSwapped();

        //resize the frame to a given size
        if (frame.size() != size()) {
            QImage scaledFrame = frame.scaled(size(), Qt::KeepAspectRatio);

            if (scaledFrame.size() != size()) {
                QImage newFrame(size(), QImage::Format_RGB888);
                newFrame.fill(Qt::black);
                QPainter painter(&newFrame);

                painter.drawImage(scaledFrame.width() < size().width() ? (newFrame.width() - scaledFrame.width()) / 2 : 0,
                                  scaledFrame.height() < size().height() ? (newFrame.height() - scaledFrame.height()) / 2 : 0, scaledFrame);

                scaledFrame = newFrame;
            }

            frame = scaledFrame;
        }
    }

    return frame;

}
