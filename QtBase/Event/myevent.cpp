#include "myevent.h"

QEvent::Type MyEvent::myType = QEvent::None;

MyEvent::MyEvent(): QEvent(myEventype())
{

}
MyEvent::~MyEvent()
{

}

QEvent::Type MyEvent::myEventype()
{

    if (myType == QEvent::None)
        myType = (QEvent::Type)QEvent::registerEventType();//注册事件

    return myType;
}
