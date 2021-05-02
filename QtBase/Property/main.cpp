#include "property.h"
#include <QApplication>

#include <QMetaProperty>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Property *p =new Property();

    p->setObjectName("property");

    const QMetaObject *metaobject = p->metaObject();

    for (int i = 0; i < metaobject->propertyCount(); ++i) {
        QMetaProperty metaproperty = metaobject->property(i);
        const char *name = metaproperty.name();
        QVariant value = p->property(name);

        qDebug()<<"name:"<<name<<"value:"<<value;
    }

    p->setData(10);
    qDebug()<<p->getData();
    qDebug()<<p->property("data");
    p->setProperty("data", 20);
    qDebug()<<p->getData();
    qDebug()<<p->property("data");

    p->setProperty("dynamic", "mydynamic");
    qDebug()<<p->property("dynamic");

    Q_CLASSINFO("Version", "3.0.0");
    qDebug()<<p->property("Version");

    return a.exec();
}
