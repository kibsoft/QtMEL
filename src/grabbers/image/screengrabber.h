#ifndef SCREENGRABBER_H
#define SCREENGRABBER_H

#include "../../qvacl_global.h"
#include "abstractimagegrabber.h"
#include <QRect>

class QVACLSHARED_EXPORT ScreenGrabber : public AbstractImageGrabber
{
    Q_OBJECT

public:
    ScreenGrabber(QObject *parent = 0);

    void setCaptureRect(const QRect &rect);
    QRect captureRect() const;

public slots:
    bool start();

signals:
    void captureRectChanged();

private:
    void grab();

    QRect m_captureRect;
};

#endif // SCREENGRABBER_H
