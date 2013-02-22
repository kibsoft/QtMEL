#ifndef MOUSEHELPER_H
#define MOUSEHELPER_H

#include "../qvacl_global.h"
#include <QObject>
#include <QPoint>

class MouseEvent {
public:
    enum MouseButton {
        LeftButton = 1,
        RightButton,
        MiddleButton
    };

    enum Type {
        MouseButtonPress = 1,
        MouseButtonRelease,
        MouseButtonClick
    };

    MouseEvent::Type type;
    MouseEvent::MouseButton button;
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

signals:
    void mouseEvent(const MouseEvent &event);

private:
    MouseHelperPrivate *pimpl;
};

#endif // MOUSEHELPER_H
