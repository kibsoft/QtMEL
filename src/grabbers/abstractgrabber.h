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

#ifndef ABSTRACTGRABBER_H
#define ABSTRACTGRABBER_H

#include "../qtmel_global.h"
#include <QObject>

//! The AbstractGrabber class is the base of all grabbers.
/*!
  The class defines the functions for the functionality shared by all grabbers. By inheriting this class, you can create custom grabbers that grab image or audio data.

  Grabbers have three states: Active, Stopped, or Paused--as defined by the AbstractGrabber::State enum.
  The current state can be changed by calling start(), stop(), suspend(), or resume().
  AbstractGrabber will emit stateChanged() whenever its state changes.
*/

class QTMELSHARED_EXPORT AbstractGrabber : public QObject {
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
        DeviceNotFoundError, /*!< Device to be grabbed was not found. */
        InvalidFormatError, /*! The format is not supported by the device. */
        DeviceOpenError /*! Unable to open device. */
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

public Q_SLOTS:
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

Q_SIGNALS:
    /*! This signal is emitted when the grabber state has changed. */
    void stateChanged();

    /*! This signal is emitted when an error occurs. */
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
