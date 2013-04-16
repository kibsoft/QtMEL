#ifndef MOUSEHELPER_P_H
#define MOUSEHELPER_P_H

#include <QObject>
#include "mousehelper.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif


class MouseHelperPrivate : public QObject {
    Q_OBJECT

public:
    explicit MouseHelperPrivate(QObject *parent = 0);
    ~MouseHelperPrivate();

    void startGrabbing();
    void stopGrabbing();

    static QPixmap cursorPixmap();

Q_SIGNALS:
    void mouseEvent(const MouseEvent &event);

private Q_SLOTS:
    void handleEvent(const MouseEvent &event);

private:
    void sendClickEvent(MouseEvent::MouseButton button, const QPoint &position);

#ifdef Q_OS_WIN
    //hook object that is returned after SetWindowsHookEx
    HHOOK m_hook;
#endif

    bool m_leftButtonDown;
    bool m_rightButtonDown;
};


#endif // MOUSEHELPER_P_H
