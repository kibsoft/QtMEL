#ifndef MOUSEHELPER_H
#define MOUSEHELPER_H

#include "../qvacl_global.h"
#include <QObject>
#include <QPoint>
#include <QPixmap>

//! The MouseEvent class contains parameters that describe a mouse event.
/*!
  Mouse events occur when a mouse button is pressed or released.
*/
class MouseEvent {
public:

    /*! This enum describes mouse buttons. */
    enum MouseButton {
        LeftButton = 1,/*!< The left button. */
        RightButton/*!< The right button. */
    };

    /*! This enum describes mouse event types. */
    enum Type {
        MouseButtonPress = 1,/*!< Mouse press. */
        MouseButtonRelease,/*!< Mouse release. */
        MouseButtonClick/*!< Mouse click. */
    };

    /*! Contains a type of an event. */
    MouseEvent::Type type;
    /*! Contains a mouse button of an event. */
    MouseEvent::MouseButton button;
    /*! Contains current cursor position. */
    QPoint position;
};

class MouseHelperPrivate;

class QVACLSHARED_EXPORT MouseHelper : public QObject
{
    Q_OBJECT
public:    
    explicit MouseHelper(QObject *parent = 0);
    ~MouseHelper();

    void startGrabbing();
    void stopGrabbing();

    static QPixmap cursorPixmap();

signals:
    void mouseEvent(const MouseEvent &event);

private:
    MouseHelperPrivate *pimpl;
};

#endif // MOUSEHELPER_H
