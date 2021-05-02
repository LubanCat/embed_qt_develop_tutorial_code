#ifndef QQLISTVIEWCHILD_H
#define QQLISTVIEWCHILD_H

#include <QWidget>
#include <QMouseEvent>

#include "qqcell.h"
#include "qqpopmenuwidget.h"


class QQListViewChild : public QQPopMenuWidget
{
    Q_OBJECT

public:
    explicit QQListViewChild(QQPopMenuWidget *parent = 0);
    ~QQListViewChild();

signals:
    void onChildDidSelected(QQListViewChild *child);
    void onChildDidDoubleClicked(QQCell *cell);
    void signalRightClicked(QQCell *cell);
protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    void paintEvent(QPaintEvent *);
public:
    QQCell *cell;
private:
    bool m_bEntered;

private:
};

#endif // QQLISTVIEWCHILD_H
