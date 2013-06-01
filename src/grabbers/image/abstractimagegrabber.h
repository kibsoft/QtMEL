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

#ifndef ABSTRACTIMAGEGRABBER_H
#define ABSTRACTIMAGEGRABBER_H

#include "../../qtmel_global.h"
#include "../abstractgrabber.h"
#include <QImage>
#include <QMutex>

class AudioTimer;
class Recorder;

//! The AbstractImageGrabber class is the base of all image grabbers.
/*!
  The class defines the functions for the functionality shared by image grabbers. By inheriting this class, you can create custom grabbers that grab images from any devices.

  AbstractImageGrabber will emit frameAvailable() whenever a new image will be available.
*/
class QTMELSHARED_EXPORT AbstractImageGrabber : public AbstractGrabber
{
    Q_OBJECT

    /*!
      This property holds the latency between capturing of images.

      Here is a pseudocode:
      @code
      forever {
        //capture an image from device
        waitForSetLatency();
      }
      @endcode

      \sa setLatency()
      \sa latency()
    */
    Q_PROPERTY(int latency READ latency WRITE setLatency NOTIFY latencyChanged)

    Q_PROPERTY(QString grabbedFrameCount READ grabbedFrameCount NOTIFY grabbedFrameCountChanged)

    friend class Recorder;

public:
    /*! Constructs an abstract image grabber with the given parent. */
    AbstractImageGrabber(QObject *parent = 0);

    /*! Destroys the abstract image grabber. */
    ~AbstractImageGrabber();

    /*!
      Sets the image capture latency. It may be used to reduce CPU usage(especially on single-core processors) and to lower frame rate.
      The default value is 0. It means that the grabber will try to capture as many frames as possible.
      \sa latency()
    */
    void setLatency(int latency);

    /*!
      Returns the current latency in milliseconds.
      \sa setLatency()
    */
    int latency() const;

    /*!
      Returns count of grabbed frames.
      \sa setGrabbedFrameCount()
    */
    int grabbedFrameCount() const;

public Q_SLOTS:
    /*! Starts data grabbing. The state() is set to AbstractGrabber::ActiveState if no errors occurred. */
    virtual bool start();

    /*! Stops data grabbing. The state() is set to AbstractGrabber::StoppedState. */
    virtual void stop();

    /*! Suspends data grabbing. The state() is set to AbstractGrabber::SuspendedState. */
    virtual void suspend();

    /*! Resumes data grabbing after a suspend(). The state() is set to AbstractGrabber::ActiveState. */
    virtual void resume();

Q_SIGNALS:
    /*!
      This signal is emitted when a new image was captured from a device.
      \param frame an image grabbed from a device.
      \param duration time of image capture.
    */
    void frameAvailable(const QImage &frame, int duration);

    /*!
      This signal is emitted immediately after the latency value has been changed.
    */
    void latencyChanged(int latency);

    /*!
      This signal is emitted immediately after the new frame has been grabbed.
    */
    void grabbedFrameCountChanged(int count);

protected:
    /*!
      Starts an image grabbing in a different thread.
      This function is called by start().
    */
    void startGrabbing();

    //! A pure virtual function.
    /*! Captures images from a device. */
    virtual void grab();
    virtual QImage captureFrame() = 0;

    void setGrabbedFrameCount(int count);

    /*!
      Sets the stop flag.
      \sa stopRequest()
    */
    void setStopRequest(bool stop);

    /*!
      Returns the stop flag. If the flag is true then grabing device is going to be stopped (if it is active)
      \sa setStopRequest()
    */
    bool isStopRequest() const;

    /*!
      Sets the pause flag.
      \sa pauseRequest()
    */
    void setPauseRequest(bool pause);

    /*!
      Returns the pause flag. If the flag is true then grabing device is going to be paused (if it is active)
      \sa setPauseRequest()
    */
    bool isPauseRequest() const;

private:
    void setTimer(AudioTimer *timer);

    int m_latency;
    int m_grabbedFrameCount;
    bool m_isStopRequest;
    bool m_isPauseRequest;
    mutable QMutex m_latencyMutex;
    mutable QMutex m_grabbedFrameCountMutex;
    mutable QMutex m_stopPauseMutex;
    AudioTimer *m_timer;
};

#endif // ABSTRACTIMAGEGRABBER_H
