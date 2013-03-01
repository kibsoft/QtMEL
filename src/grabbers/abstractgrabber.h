#ifndef ABSTRACTGRABBER_H
#define ABSTRACTGRABBER_H

#include "../qvacl_global.h"
#include <QObject>

//! The AbstractGrabber class is the base of all grabbers.
/*!
  The class defines the functions for the functionality shared by all grabbers. By inheriting this class, you can create custom grabbers that grab image or audio data.

  Grabbers have three states: Active, Stopped, or Paused--as defined by the AbstractGrabber::State enum.
  The current state can be changed by calling start(), stop(), suspend(), or resume().
  AbstractGrabber will emit stateChanged() whenever its state changes.
*/

class QVACLSHARED_EXPORT AbstractGrabber : public QObject {
    Q_OBJECT

public:
    /*! This enum describes the state of the grabber. */
    enum State {
        ActiveState = 0, /*!< The grabber is capturing data, this state is set after start() is called. */
        SuspendedState, /*!< The grabber is suspended, this state will only be entered after suspend() is called. */
        StoppedState /*!< The grabber doesn't do anything. */
    };

    /*! This enum describes grabber errors. */
    enum Error {
        NoError = 0, /*!< No error occurred. */
        InvalidConfigurationError, /*!< Initial data for the grabber were invalid. For example, capture rectangle from ScreenGrabber. */
        DeviceNotFoundError /*!< Device to be grabbed was not found. */
    };

    /*! Constructs an abstract grabber with the given parent. */
    AbstractGrabber(QObject *parent = 0);

    /*! Destroys the abstract grabber. */
    virtual ~AbstractGrabber();

    /*!
      Returns the state of data processing.
      \sa stateChanged()
    */
    AbstractGrabber::State state() const;

    /*!
      Returns the last error that occurred. If no error occurred, returns AbstractGrabber::NoError.
      \sa errorString()
    */
    AbstractGrabber::Error error() const;

    /*!
      Returns a human-readable description of the last error that occurred.
      \sa error()
    */
    QString errorString() const;

public slots:
    //! A pure virtual function.
    /*! Starts data grabbing. The state() is set to AbstractGrabber::ActiveState if no errors occurred. */
    virtual bool start() = 0;

    //! A pure virtual function.
    /*! Stops data grabbing. The state() is set to AbstractGrabber::StoppedState. */
    virtual void stop() = 0;

    //! A pure virtual function.
    /*! Suspends data grabbing. The state() is set to AbstractGrabber::SuspendedState. */
    virtual void suspend() = 0;

    //! A pure virtual function.
    /*! Resumes data grabbing after a suspend(). The state() is set to AbstractGrabber::ActiveState. */
    virtual void resume() = 0;

signals:
    /*! This signal is emitted when the grabber state has changed. */
    void stateChanged();

    void error(AbstractGrabber::Error error);

protected:
    /*!
      Sets the grabber's state to the given state.
      \sa state()
    */
    void setState(AbstractGrabber::State state);

    void setError(AbstractGrabber::Error errorCode, const QString &errorString);

private:
    AbstractGrabber::State m_state;
    AbstractGrabber::Error m_error;
    QString m_errorString;
};

#endif // ABSTRACTGRABBER_H
