#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush(QColor(0, 255, 0, 125));

    //笔刷
    pen.setStyle(Qt::DotLine);
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::NoBrush);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(5,10,90,90);

    pen.setStyle(Qt::SolidLine);
    brush.setColor(Qt::red);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(105,10,90,90);

    pen.setStyle(Qt::DashLine);
    brush.setColor(Qt::yellow);
    brush.setStyle(Qt::Dense3Pattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(205,10,90,90);

    pen.setStyle(Qt::DotLine);
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::HorPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(305,10,90,90);

    pen.setStyle(Qt::DashDotLine);
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::CrossPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(405,10,90,90);

    pen.setStyle(Qt::DashDotDotLine);
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::BDiagPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(505,10,90,90);

    pen.setStyle(Qt::SolidLine);
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::DiagCrossPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(605,10,90,90);

    pen.setStyle(Qt::NoPen);
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::NoBrush);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(705,10,90,90);

    //画直线
    pen.setWidth(5);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt :: SquareCap);
    painter.setPen(pen);
    painter.drawLine(QPointF(5, 110), QPointF(195, 110));
    pen.setStyle(Qt::DashLine);
    pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen);
    painter.drawLine(QPointF(5, 130), QPointF(195, 130));

    pen.setStyle(Qt::DotLine);
    pen.setCapStyle(Qt :: FlatCap);
    painter.setPen(pen);
    painter.drawLine(QPointF(5, 150), QPointF(195, 150));
    pen.setStyle(Qt::DashDotLine);
    pen.setColor(QColor(255, 255, 0));
    painter.setPen(pen);
    painter.drawLine(QPointF(5, 170), QPointF(195, 170));

    pen.setStyle(Qt::DashDotDotLine);
    pen.setCapStyle(Qt :: RoundCap);
    painter.setPen(pen);
    painter.drawLine(QPointF(5, 190), QPointF(195, 190));
    pen.setStyle(Qt::CustomDashLine);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(QPointF(5, 210), QPointF(195, 210));


    //颜色
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
        {
            painter.setBrush(QColor(255-j*50, i*50, i*j*10, 100));
            painter.drawRect(205+i*30, 110+j*30, 30, 30);
        }

    //点
    painter.setPen(QColor(255,0,0));
    painter.setBrush(QColor(255,0,0));
    QPolygonF points;
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
        {
            painter.drawPoint(QPoint(305+i*10,110+j*10));
            points.append(QPoint(405+i*10,110+j*10));
        }

    painter.setPen(QColor(0,0,255));
    painter.setBrush(QColor(0,0,255));
    painter.drawPoints(points);

    //画矩形
    pen.setColor(QColor(255, 0, 0));
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(0, 255, 0, 125));
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(505, 110, 90, 90);

    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(255, 0, 0));
    brush.setStyle(Qt::NoBrush);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(605, 110, 90, 90);

    QRectF rectangle(705.0, 110.0, 90.0, 90.0);
    painter.drawRoundedRect(rectangle, 20.0, 20.0);

    //画弧线
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    int startAngle = 30 * 16;//起始角度
    int spanAngle = 120 * 16;//跨越度数
    painter.drawArc(QRectF(5.0, 210.0, 90.0, 90.0), startAngle, spanAngle);

    //扇形
    painter.drawPie(QRectF(105.0, 210.0, 90.0, 90.0), startAngle, spanAngle);

    //带弦的弧
    painter.drawChord(QRect(205, 210, 90, 90), startAngle, spanAngle);

    //椭圆
    painter.drawEllipse(QRectF(305,220,90,60));
    //圆
    painter.drawEllipse(QRectF(405,210,90,90));

    //折线
    QPointF Polylinepoints[5] = {
        QPointF(505.0, 210.0),
        QPointF(555.0, 290.0),
        QPointF(535.0, 275.0),
        QPointF(595.0, 295.0),
        QPointF(505.0, 215.0),
    };
    painter.drawPolyline(Polylinepoints, 5);

    //多边形
    QPointF Polygonpoints[4] = {
        QPointF(610.0, 280.0),
        QPointF(620.0, 210.0),
        QPointF(680.0, 230.0),
        QPointF(690.0, 270.0),
    };
    painter.drawConvexPolygon(Polygonpoints, 4);

    //线性渐变
    QLinearGradient linearGradient(QPointF(40, 190), QPointF(70, 190));
    linearGradient.setColorAt(0, Qt::yellow);
    linearGradient.setColorAt(0.5, Qt::red);
    linearGradient.setColorAt(1, Qt::green);
    linearGradient.setSpread(QGradient::RepeatSpread);
    painter.setBrush(linearGradient);
    painter.drawRect(5, 310, 90, 90);

    //辐射渐变
    QRadialGradient radialGradient(QPointF(150, 350),40,QPointF(550,40));
    radialGradient.setColorAt(0, QColor(255, 255, 100, 150));
    radialGradient.setColorAt(1, QColor(0, 0, 0, 50));
    painter.setBrush(radialGradient);
    painter.drawEllipse(QPointF(150, 350), 40, 40);

    //锥形渐变
    QConicalGradient conicalGradient(QPointF(250, 350), 100);
    conicalGradient.setColorAt(0.2, Qt::cyan);
    conicalGradient.setColorAt(0.9, Qt::black);
    painter.setBrush(conicalGradient);
    painter.drawEllipse(QPointF(250, 350), 40, 40);

    //设置一个矩形
    QRectF rect(305, 310, 190, 90);
    painter.drawRect(rect);
    painter.setPen(QColor(Qt::red));
    painter.drawText(rect, Qt::AlignHCenter, "野火电子");

    //绘制文字
    QFont font("宋体", 10, QFont::Bold, true);
    font.setLetterSpacing(QFont::AbsoluteSpacing,5);
    painter.setFont(font);
    painter.drawText(310, 350, "Qt 嵌入式教程");

    //绘制文字
    painter.setPen(Qt::green);
    font.setLetterSpacing(QFont::AbsoluteSpacing,0);
    font.setBold(false);
    font.setFamily("黑体");
    font.setPointSize(10);
    font.setItalic(true);
    painter.setFont(font);
    painter.drawText(310, 380, "显示文字 显示文字");

    //简单的使用路径
    QPainterPath path1;
    path1.addEllipse(510, 320, 50, 50);
    path1.lineTo(505, 380);
    painter.setPen(Qt::blue);
    painter.setBrush(Qt::red);
    painter.drawPath(path1);

    //复制图形
    QPainterPath path2;
    path2.addPath(path1);
    path2.translate(70,0);
    painter.drawPath(path2);

    QPainterPath path4;
    painter.setBrush(Qt::transparent);
    path4.lineTo(0,105);
    path4.lineTo(this->width(),105);
    path4.lineTo(this->width(),205);
    path4.lineTo(0,205);
    path4.lineTo(0,305);
    path4.lineTo(this->width(),305);
    path4.lineTo(this->width(),405);
    path4.lineTo(0,405);
    painter.drawPath(path4);

    //简单绘制图片
    QPixmap pix;
    pix.load(":/image/test.png");
    painter.drawPixmap(650, 305, 25, 25, pix);

    //缩放
    pix = pix.scaled(pix.width()*2, pix.height()*2,Qt::KeepAspectRatio);
    painter.drawPixmap(650, 330, pix);
}
