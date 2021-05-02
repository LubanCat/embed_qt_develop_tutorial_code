/******************************************************************
 Copyright (C) 2016 - All Rights Reserved by
 重庆重邮汇测通信技术有限公司
 文 件 名 : rotatingstackedwidget.cpp --- RotatingStackedWidget
 作 者    : Niyh
 编写日期 : 2016/10/27
 说 明    : 控件来自网络
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
 倪又华    2016/10/27    1.0.0.0 1     文件创建
*******************************************************************/
#include "rotatingstackedwidget.h"

#include <QVariant>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QTransform>

RotatingStackedWidget::RotatingStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    iRotateVal = 0;
    isAnimating = false;
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

RotatingStackedWidget::~RotatingStackedWidget()
{

}

/**
 * @brief RotatingStackedWidget::paintEvent
 * 绘制
 * @param event
 */
void RotatingStackedWidget::paintEvent(QPaintEvent *event)
{
    if (isAnimating)
    {
        if (iRotateVal > 90)
        {
            QPixmap pixmap(widget(nextIndex)->size());
            widget(nextIndex)->render(&pixmap);
            QPainter painter(this);

            QTransform transform;
            transform.translate(width() / 2, 0);
            transform.rotate(iRotateVal + 180, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, pixmap);
        }
        else
        {
            QPixmap pixmap(currentWidget()->size());
            currentWidget()->render(&pixmap);
            QPainter painter(this);

            QTransform transform;
            transform.translate(width() / 2, 0);
            transform.rotate(iRotateVal, Qt::YAxis);
            painter.setTransform(transform);
            painter.drawPixmap(-1 * width() / 2, 0, pixmap);
        }
    }
    else {
        QWidget::paintEvent(event);
    }
}

/**
 * @brief RotatingStackedWidget::nextPage
 * 下一个page
 */
void RotatingStackedWidget::nextPage()
{
    rotate((currentIndex() + 1) >= count() ? 0 : (currentIndex() + 1));
}

/**
 * @brief RotatingStackedWidget::rotate
 * 执行翻转
 * @param index
 */
void RotatingStackedWidget::rotate(int index)
{
    if(isAnimating) return;
    nextIndex = index;
    int offsetx=frameRect().width();
    int offsety=frameRect().height();
    widget(index)->setGeometry ( 0,  0, offsetx, offsety );

    QPropertyAnimation *animnow = new QPropertyAnimation(this,"rotateVal");
    animnow->setDuration(500);
    animnow->setEasingCurve(QEasingCurve::Linear);
    animnow->setStartValue(0);
    animnow->setEndValue(180);
    connect(animnow, SIGNAL(valueChanged(QVariant)), this, SLOT(valChanged(QVariant)));
    connect(animnow, SIGNAL(finished()), this, SLOT(animDone()));
    currentWidget()->hide();
    isAnimating = true;
    animnow->start();
}

/**
 * @brief RotatingStackedWidget::rotateVal
 * @return
 */
float RotatingStackedWidget::rotateVal()
{
    return iRotateVal;
}

/**
 * @brief RotatingStackedWidget::setRotateVal
 * @param fl
 */
void RotatingStackedWidget::setRotateVal(float fl)
{
    iRotateVal = fl;
}

void RotatingStackedWidget::valChanged(QVariant)
{
    repaint();
}

/**
 * @brief RotatingStackedWidget::animDone
 *
 */
void RotatingStackedWidget::animDone()
{
    iRotateVal = 0;
    isAnimating = false;
    widget(nextIndex)->show();
    widget(nextIndex)->raise();;
    setCurrentWidget(widget(nextIndex));
    repaint();
}
