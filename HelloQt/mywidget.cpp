#include "mywidget.h"

#include <QLabel>
#include <QBoxLayout>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("hello widget");

    QLabel *label =new QLabel("hello widget");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *verLayout = new QVBoxLayout(this);
    verLayout->setContentsMargins(0, 0, 0, 0);
    verLayout->setSpacing(0);

    verLayout->addWidget(label);
}
