#include "mousehelper.h"
#include "mousehelper_p.h"

MouseHelper::MouseHelper(QObject *parent) :
    QObject(parent)
  , pimpl(new MouseHelperPrivate(this))
{
    connect(pimpl, SIGNAL(mouseEvent(MouseEvent)), this, SIGNAL(mouseEvent(MouseEvent)));
}

MouseHelper::~MouseHelper()
{
}

void MouseHelper::startGrabbing()
{
    pimpl->startGrabbing();
}

void MouseHelper::stopGrabbing()
{
    pimpl->stopGrabbing();
}
