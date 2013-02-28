#ifndef SCREENGRABBER_H
#define SCREENGRABBER_H

#include "../../qvacl_global.h"
#include "abstractimagegrabber.h"
#include <QRect>
#include <QMutex>

class QVACLSHARED_EXPORT ScreenGrabber : public AbstractImageGrabber
{
    Q_OBJECT

    Q_PROPERTY(QRect captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)
    Q_PROPERTY(bool captureCursor READ captureCursor WRITE setCaptureCursor NOTIFY captureCursorChanged)
public:
    ScreenGrabber(QObject *parent = 0);

    void setCaptureRect(const QRect &rect);
    QRect captureRect() const;

    void setCaptureCursor(bool capture);
    bool captureCursor() const;

public slots:
    bool start();

signals:
    void captureRectChanged(const QRect &rect);
    void captureCursorChanged(bool capture);

private:
    void grab();

    QRect m_captureRect;
    bool m_captureCursor;

    mutable QMutex m_captureRectMutex;
    mutable QMutex m_captureCursorMutex;
};

#endif // SCREENGRABBER_H
