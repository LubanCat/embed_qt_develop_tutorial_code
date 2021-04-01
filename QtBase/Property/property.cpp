#include "property.h"

Property::Property(QObject *parent) : QObject(parent)
{
    m_data=24;
}
int Property::getData()
{
    return m_data;
}
void Property::setData(int data)
{
    this->m_data=data;
}
