#ifndef PROPERTY_H
#define PROPERTY_H

#include <QObject>

class Property : public QObject
{
    Q_PROPERTY(int data READ getData WRITE setData NOTIFY dataChangeed)
    Q_OBJECT
public:
    explicit Property(QObject *parent = nullptr);

    int getData();
    void setData(int data);

signals:
    void dataChangeed(int data);

public slots:

private:
    int m_data;
};

#endif // PROPERTY_H
