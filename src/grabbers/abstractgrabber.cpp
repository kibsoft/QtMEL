#include "abstractgrabber.h"


AbstractGrabber::AbstractGrabber(QObject *parent)
    : QObject(parent)
    , m_state(AbstractGrabber::StoppedState)
{
}

AbstractGrabber::~AbstractGrabber()
{
}

AbstractGrabber::State AbstractGrabber::state() const
{
    return m_state;
}
