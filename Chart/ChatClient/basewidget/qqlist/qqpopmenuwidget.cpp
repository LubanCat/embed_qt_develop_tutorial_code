#include "qqpopmenuwidget.h"

QQPopMenuWidget::QQPopMenuWidget(QWidget *parent) :
    CustomWidget(parent)
{
}

void QQPopMenuWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        if (NULL == popMenu)
        {
            return ;
        }

        emit onpopmenuwillshow(this, popMenu);
        popMenu->exec(QCursor::pos());
    }
}

void QQPopMenuWidget::setPopMenu(QMenu *menu)
{
    popMenu = menu;
}
