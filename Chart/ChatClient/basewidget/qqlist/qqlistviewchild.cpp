#include "qqlistviewchild.h"
#include "global.h"

#include <QDebug>
#include <QEvent>
#include <QPainter>

QQListViewChild::QQListViewChild(QQPopMenuWidget *parent) :
    QQPopMenuWidget(parent)
{
    m_bEntered = false;
}

QQListViewChild::~QQListViewChild()
{
}

void QQListViewChild::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        emit onChildDidSelected(this);
    }
    else if (Qt::RightButton == ev->button()) {
        if (NULL == this->cell) return;
        Q_EMIT signalRightClicked(this->cell);

        QQPopMenuWidget::mousePressEvent(ev);
    }
}

void QQListViewChild::mouseDoubleClickEvent(QMouseEvent * /*event*/)
{
    emit onChildDidDoubleClicked(this->cell);
}

void QQListViewChild::enterEvent(QEvent *event)
{
    m_bEntered = true;
    update();
    QWidget::enterEvent(event);
}

void QQListViewChild::leaveEvent(QEvent *event)
{
    m_bEntered = false;
    update();
    QWidget::leaveEvent(event);
}

void QQListViewChild::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(this->rect(), m_bEntered ? QColor("#D9D8D7") : QColor("transparent"));
    QPixmap pixmap(cell->iconPath);
    if (pixmap.isNull()) pixmap = QPixmap(":/resource/head/1.bmp");

    // 如果当前是离线绘制灰度图
    if (OffLine == cell->status) {
        pixmap = myHelper::ChangeGrayPixmap(pixmap.toImage());
    }

    // 绘制头像
    painter.drawPixmap(10, 10, 40, 40, pixmap);

    int w = this->rect().width();
    int h = this->rect().height();

    QFont font("微软雅黑", 14);
    QPen pen;

    // 绘制名字
    pen.setColor(QColor("#000000"));
    font.setPixelSize(12);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(70, 0, w - 70, h / 2 - 2, Qt::AlignBottom, cell->name);

    // 绘制subtitile
    pen.setColor(QColor("#666666"));
    font.setPixelSize(10);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(70, h / 2 + 2, w - 70, h / 2 - 2, Qt::AlignTop, cell->subTitle);
}
