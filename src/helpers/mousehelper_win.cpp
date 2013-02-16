#include "mousehelper_p.h"
#include <QtGui/qwindowdefs.h>
#include <QCursor>

class EventHandler : public QObject {
    Q_OBJECT
public:
    friend LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);

    static EventHandler* instance();
    static void destroy();

signals:
    void mouseEvent(const MouseEvent &event);

private:
    explicit EventHandler(QObject *parent = 0) : QObject(parent) {}
    EventHandler(const EventHandler &) {}
    EventHandler& operator=(const EventHandler &) { return *this; }

    static EventHandler* m_instance;
};

EventHandler* EventHandler::m_instance = 0;

EventHandler *EventHandler::instance()
{
    if (!m_instance)
        m_instance = new EventHandler;

    return m_instance;
}

void EventHandler::destroy()
{
    if (m_instance) {
        delete m_instance;
        m_instance = 0;
    }
}

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);

MouseHelperPrivate::MouseHelperPrivate(QObject *parent)
    : QObject(parent)
    , m_hook(0)
    , m_leftButtonDown(false)
    , m_rightButtonDown(false)
{
}

MouseHelperPrivate::~MouseHelperPrivate()
{
    stopGrabbing();
}

void MouseHelperPrivate::startGrabbing()
{
    connect(EventHandler::instance(), SIGNAL(mouseEvent(MouseEvent)), this, SLOT(handleEvent(MouseEvent)), Qt::UniqueConnection);

    m_hook = SetWindowsHookEx(WH_MOUSE_LL, HookProc, qWinAppInst(), NULL);
}

void MouseHelperPrivate::stopGrabbing()
{
    if (m_hook) {
        UnhookWindowsHookEx(m_hook);
        m_hook = 0;
    }

    disconnect(EventHandler::instance(), SIGNAL(mouseEvent(MouseEvent)), this, SLOT(handleEvent(MouseEvent)));
    EventHandler::destroy();
}

void MouseHelperPrivate::handleEvent(const MouseEvent &event)
{
    emit mouseEvent(event);

    switch (event.type) {
    case MouseEvent::MouseButtonPress:
        (event.button == MouseEvent::LeftButton ? m_leftButtonDown : m_rightButtonDown) = true;
        break;

    case MouseEvent::MouseButtonRelease:
        sendClickEvent(event.button, event.position);
        break;

    //avoid compiler warning
    case MouseEvent::MouseButtonClick:
        break;
    }
}

void MouseHelperPrivate::sendClickEvent(MouseEvent::MouseButton button, const QPoint &position)
{
    bool *isButtonDown = &(button == MouseEvent::LeftButton ? m_leftButtonDown : m_rightButtonDown);
    if (*isButtonDown) {
        *isButtonDown = false;

        MouseEvent clickEvent;
        clickEvent.type = MouseEvent::MouseButtonClick;
        clickEvent.button = button;
        clickEvent.position = position;

        emit mouseEvent(clickEvent);
    }
}

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    MouseEvent event;
    event.position = QCursor::pos();

    switch (wParam) {
    case WM_LBUTTONDOWN: {
        event.type = MouseEvent::MouseButtonPress;
        event.button = MouseEvent::LeftButton;
        emit EventHandler::instance()->mouseEvent(event);
    } break;

    case WM_LBUTTONUP: {
        event.type = MouseEvent::MouseButtonRelease;
        event.button = MouseEvent::LeftButton;
        emit EventHandler::instance()->mouseEvent(event);
    } break;

    case WM_RBUTTONDOWN: {
        event.type = MouseEvent::MouseButtonPress;
        event.button = MouseEvent::RightButton;
        emit EventHandler::instance()->mouseEvent(event);
    } break;

    case WM_RBUTTONUP: {
        event.type = MouseEvent::MouseButtonRelease;
        event.button = MouseEvent::RightButton;
        emit EventHandler::instance()->mouseEvent(event);
    } break;
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


#include <mousehelper_win.moc>
