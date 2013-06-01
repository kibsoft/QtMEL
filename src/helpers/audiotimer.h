#ifndef AUDIOTIMER_H
#define AUDIOTIMER_H

#include <QObject>

class AudioGrabber;

class AudioTimer : public QObject
{
    Q_OBJECT
public:
    explicit AudioTimer(AudioGrabber *grabber, QObject *parent = 0);

    int elapsed() const;
private:
    AudioGrabber *m_grabber;
};

#endif // AUDIOTIMER_H
