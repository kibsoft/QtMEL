#ifndef ABSTRACTGRABBER_H
#define ABSTRACTGRABBER_H

#include "qvacl_global.h"
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

    /*! Constructs an abstract grabber with the given parent. */
    AbstractGrabber(QObject *parent = 0);

    /*! Destroys the abstract grabber. */
    virtual ~AbstractGrabber();


    //! A pure virtual function.
    /*! Starts data grabbing. The state() is set to Active if no errors occurred. */
    virtual void start() = 0;

    //! A pure virtual function.
    /*! Stops data grabbing. The state() is set to AbstractGrabber::StoppedState. */
    virtual void stop() = 0;

    //! A pure virtual function.
    /*! Suspends data grabbing. The state() is set to AbstractGrabber::SuspendedState. */
    virtual void suspend() = 0;

    //! A pure virtual function.
    /*! Resumes data grabbing after a suspend(). The state() is set to AbstractGrabber::ActiveState. */
    virtual void resume() = 0;

    /*! Returns the state of data processing. */
    AbstractGrabber::State state() const;

signals:
    /*! This signal is emitted when the grabber state has changed. */
    void stateChanged();

protected:
    AbstractGrabber::State m_state;
};

#endif // ABSTRACTGRABBER_H
