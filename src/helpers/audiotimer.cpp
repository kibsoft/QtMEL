#include "audiotimer.h"

#include "../grabbers/audio/audiograbber.h"

AudioTimer::AudioTimer(AudioGrabber *grabber, QObject *parent) :
    QObject(parent)
  , m_grabber(grabber)
{
}

int AudioTimer::elapsed() const
{
    return m_grabber->elapsedMilliseconds();
}
