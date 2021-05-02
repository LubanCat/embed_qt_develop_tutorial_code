#ifndef MYWidget_H
#define MYWidget_H

#include <QWidget>
#include <QObject>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // MYWidget_H
