#ifndef SCREENGRABBER_H
#define SCREENGRABBER_H

#include "../../qvacl_global.h"
#include "abstractimagegrabber.h"
#include <QRect>
#include <QMutex>

//! The ScreenGrabber provides a desktop screen image grabber.
/*!
  The class is used to capture full or part screen images from desktop. You can capture the screen with or without the cursor, use setCaptureCursor() for that.
  In order to capture part of the screen use setCaptureRect() function.

  ScreenGrabber will emit frameAvailable() whenever a new image will be available.

  Here is an example of the ScreenGrabber usage:
  @code
  ScreenGrabber *grabber = new ScreenGrabber(this);
  grabber->setCaptureRect(QRect(0, 0, 640, 480));//capture the rectangle from (0, 0) point of the screen with the size 640x480
  grabber->setCaptureRect(false);//we don't want to see the cursor in the final video
  grabber->start();
  @endcode
*/
class QVACLSHARED_EXPORT ScreenGrabber : public AbstractImageGrabber
{
    Q_OBJECT

    /*!
      This property holds the capture rectangle.
      \sa setCaptureRect()
      \sa captureRect()
    */
    Q_PROPERTY(QRect captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)
    /*!
      This property holds whether to capture the cursor while grabbing.
      \sa setCaptureCursor()
      \sa captureCursor()
    */
    Q_PROPERTY(bool captureCursor READ captureCursor WRITE setCaptureCursor NOTIFY captureCursorChanged)
public:
    /*! Constructs a screen grabber with the given parent. */
    explicit ScreenGrabber(QObject *parent = 0);

    /*! Destroys the screen grabber. */
    ~ScreenGrabber();

    /*!
      Sets the capture rectangle. You can use it to capture part of the screen.
      \sa captureRect()
    */
    void setCaptureRect(const QRect &rect);
    /*!
      Returns the current capture rectangle.
      \sa setCaptureRect()
    */
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
