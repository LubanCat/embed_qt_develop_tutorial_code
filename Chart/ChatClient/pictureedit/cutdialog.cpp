#include "cutdialog.h"
#include <QDebug>

CutDialog::CutDialog(QWidget *parent) :
    QFrame(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    m_left         = false;
    m_right        = false;
    m_top          = false;
    m_bottom       = false;
    m_isMoving     = false;
    m_mouse_down   = false;

    setMouseTracking(true);
    setGeometry(0, 0, 150, 150);

    label = new QLabel(this);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(label);
    layout->setMargin(0);
    setLayout(layout);
    label->setMouseTracking(true);
}

CutDialog::~CutDialog()
{

}

void CutDialog::resizeEvent(QResizeEvent */*event*/)
{	
}

void CutDialog::mousePressEvent(QMouseEvent *event)
{
    m_startPoint = event->pos();
    m_mouse_down = event->button() == Qt::LeftButton;
}

void CutDialog::paintEvent(QPaintEvent */*event*/)
{
    QPainter paint(this);
    QPen pen;
    QVector<qreal> dashes;
    qreal space = 3;
    dashes << 5 << space << 5 <<space;
    pen.setDashPattern(dashes);
    pen.setColor(Qt::white);

    QPen pen0;
    pen0.setColor(QColor(54,158,254,120));
    pen0.setWidth(2);
    paint.setPen(pen0);
    paint.drawRect(1,0,width()-2,width()-2);

    paint.setPen(pen);
    int x_pos = (int) width()/3.0;
    int y_pos = (int) height()/3.0;
    paint.drawLine(x_pos, 0, x_pos, height());
    paint.drawLine(2*x_pos, 0, 2*x_pos, height());
    paint.drawLine(0, y_pos, width(), y_pos);
    paint.drawLine(0, 2*y_pos, width(), 2*y_pos);
    paint.drawEllipse(0,0,width(), width());
}

QRect CutDialog::getResizeGem(QRect oldgeo, QPoint mousePoint)
{
    QRect g = oldgeo;
    bool lorr = m_left | m_right;
    bool torb = m_top | m_bottom;
    int dx = mousePoint.x() - m_startPoint.x();
    int dy = mousePoint.y() - m_startPoint.y();
    if(lorr && torb)
    {
        int maxLen = qMin(qAbs(dx),qAbs(dy));
        if(m_left && m_top && dx*dy > 0)
        {
            g.setLeft(dx >0 ?g.left() + maxLen : g.left() - maxLen);
            g.setTop(dy >0? g.top() + maxLen : g.top() - maxLen);
        }
        if(m_right && m_top && dx*dy < 0)
        {
            g.setRight(dx>0 ? g.right() + maxLen:g.right() - maxLen);
            g.setTop(dy >0? g.top() + maxLen : g.top() - maxLen);
        }
        if(m_right && m_bottom && dx*dy > 0)
        {
            g.setRight(dx>0 ? g.right() + maxLen:g.right() - maxLen);
            g.setBottom(dy >0? g.bottom() + maxLen : g.bottom() - maxLen);
        }
        if(m_left && m_bottom && dx*dy < 0)
        {
            g.setLeft(dx >0 ?g.left() + maxLen : g.left() - maxLen);
            g.setBottom(dy >0? g.bottom() + maxLen : g.bottom() - maxLen);
        }
        return g;
    }
    else if(lorr)
    {
        if(m_left)
            g.setLeft(g.left() + dx);
        if(m_right)
            g.setRight(g.right() + dx);
        int len = g.width() - oldgeo.width();
        int intHight = (int) len/2.0;

        g.setTop(g.top() - intHight);
        g.setBottom(g.bottom() + len - intHight);
    }
    else if(torb)
    {
        if(m_bottom)
            g.setBottom(g.bottom() + dy);
        if(m_top)
            g.setTop(g.top() + dy);
        int dheigt = g.height() - oldgeo.height();
        int intWidth = (int) dheigt/2.0;

        g.setLeft(g.left() - intWidth);
        g.setRight(g.right() + dheigt - intWidth);
    }
    return g;
}

void CutDialog::mouseMoveEvent(QMouseEvent * event)
{

    QPoint dragPoint = event->pos();
    int x = event->x();
    int y = event->y();

    if (m_mouse_down)
    {
        QRect newRect = getResizeGem(geometry(), dragPoint);
        if(parentWidget()->rect().contains(newRect))
            setGeometry(newRect);
        m_startPoint = QPoint(!m_right ? m_startPoint.x() :
                                         event->x(),
                              !m_bottom ? m_startPoint.y():
                                          event->y());
        if(!m_left && !m_right && !m_bottom && !m_top)
        {
            QPoint p = QPoint((pos().x()+dragPoint.x() - m_startPoint.x()),
                              (pos().y()+dragPoint.y() - m_startPoint.y()));
            QPoint dragEndge = p;
            dragEndge.setX(dragEndge.x() + rect().width());
            dragEndge.setY(dragEndge.y() + rect().height());
            p.setX(p.x() < 0 ? 0 : p.x());
            p.setX(dragEndge.x() > parentWidget()->width() ?
                       parentWidget()->width()-rect().width():
                       p.x());

            p.setY(p.y() < 0 ? 0 : p.y());
            p.setY(dragEndge.y() > parentWidget()->height() ?
                       parentWidget()->height()-rect().height() :
                       p.y());
            move(p);
        }
        else {

        }
    }
    else
    {
        QRect r = rect();
        m_left = qAbs(x - r.left())  < 5;
        m_right = qAbs(x - r.right()) < 5;
        m_bottom = qAbs(y - r.bottom()) < 5;
        m_top = qAbs(y - r.top()) < 5;
        bool lorr = m_left | m_right;
        bool torb = m_top | m_bottom;
        if(lorr && torb)
        {
            if((m_left && m_top) || (m_right && m_bottom))
            {
                setCursor(Qt::SizeFDiagCursor);
            }
            else
                setCursor(Qt::SizeBDiagCursor);
        }
        else if(lorr)
            setCursor(Qt::SizeHorCursor);
        else if(torb)
            setCursor(Qt::SizeVerCursor);
        else
        {
            setCursor(Qt::SizeAllCursor);
            m_bottom = m_left = m_right = m_top = false;
        }
    }
}

void CutDialog::mouseReleaseEvent(QMouseEvent */*event*/)
{
    m_mouse_down = false;
    // 发送移动信号
    Q_EMIT signalMoved();
}
