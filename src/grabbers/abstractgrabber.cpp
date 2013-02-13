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

        emit stateChanged();
    }
}

void AbstractGrabber::setError(AbstractGrabber::Error errorCode, const QString &errorString)
{
    m_error = errorCode;
    m_errorString = errorString;

    emit error(errorCode);
}
