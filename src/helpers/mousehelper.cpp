#include "mousehelper.h"
#include "mousehelper_p.h"

MouseHelper* MouseHelper::m_instance = 0;

MouseHelper::MouseHelper(QObject *parent) :
    QObject(parent)
  , pimpl(new MouseHelperPrivate(this))
{
    connect(pimpl, SIGNAL(mouseEvent(MouseEvent)), this, SIGNAL(mouseEvent(MouseEvent)));
}

MouseHelper *MouseHelper::instance()
{
    if (!m_instance)
        m_instance = new MouseHelper;

    return m_instance;
}

void MouseHelper::startGrabbing()
{
    pimpl->startGrabbing();
}

void MouseHelper::stopGrabbing()
{
    pimpl->stopGrabbing();
}
