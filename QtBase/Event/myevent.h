#ifndef MYEVENT_H
#define MYEVENT_H

#include <QObject>
#include <QEvent>

class MyEvent : public QEvent
{
public:
    MyEvent();
    ~MyEvent();

    static Type myEventype();

private:
    static Type myType;
};

#endif // MYEVENT_H
