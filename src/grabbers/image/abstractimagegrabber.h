#ifndef ABSTRACTIMAGEGRABBER_H
#define ABSTRACTIMAGEGRABBER_H

#include "../../qvacl_global.h"
#include "../abstractgrabber.h"
#include <QImage>
#include <QMutex>

//! The AbstractImageGrabber class is the base of all image grabbers.
/*!
  The class defines the functions for the functionality shared by image grabbers. By inheriting this class, you can create custom grabbers that grab images from any devices.

  AbstractImageGrabber will emit frameAvailable() whenever a new image will be available.
*/
class QVACLSHARED_EXPORT AbstractImageGrabber : public AbstractGrabber
{
    Q_OBJECT

    Q_PROPERTY(int latency READ latency WRITE setLatency NOTIFY latencyChanged)

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

public slots:
    /*! Starts data grabbing. The state() is set to AbstractGrabber::ActiveState if no errors occurred. */
    virtual bool start();

    /*! Stops data grabbing. The state() is set to AbstractGrabber::StoppedState. */
    virtual void stop();

    /*! Suspends data grabbing. The state() is set to AbstractGrabber::SuspendedState. */
    virtual void suspend();

    /*! Resumes data grabbing after a suspend(). The state() is set to AbstractGrabber::ActiveState. */
    virtual void resume();

signals:
    /*!
      This signal is emitted when a new image was captured from a device.
      \param frame an image grabbed from a device.
      \param duration a time of image capture.
     */
    void frameAvailable(const QImage &frame, int duration);

    /*!
      This signal is emitted immediately after the latency value has been changed.
     */
    void latencyChanged(int latency);

protected:
    /*!
      Starts an image grabbing in a different thread.
      This function is called by start().
    */
    void startGrabbing();

    //! A pure virtual function.
    /*! Captures images from a device. */
    virtual void grab() = 0;

    /*!
      Sets the stop flag.
      \sa stopRequest()
    */
    void setStopRequest(bool stop);

    /*!
      Returns the stop flag. If the flag is true then grabing device is going to be stopped (if it is active)
      \sa setStopRequest()
    */
    bool stopRequest() const;

    /*!
      Sets the pause flag.
      \sa pauseRequest()
    */
    void setPauseRequest(bool pause);

    /*!
      Returns the pause flag. If the flag is true then grabing device is going to be paused (if it is active)
      \sa setPauseRequest()
    */
    bool pauseRequest() const;

private:
    int m_latency;
    bool m_stopRequest;
    bool m_pauseRequest;
    mutable QMutex m_stopPauseMutex;
};

#endif // ABSTRACTIMAGEGRABBER_H
