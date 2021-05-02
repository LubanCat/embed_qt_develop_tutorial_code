#include "photoshotdialog.h"
#include "cutdialog.h"
#include <QDebug>

PhotoShotDialog::PhotoShotDialog(QWidget *parent)
	:QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);

	dialog = new CutDialog(this);
	dialog->show();
    setGeometry(0, 0, parentWidget()->width(), parentWidget()->height());
    connect(dialog, SIGNAL(signalMoved()), this, SLOT(sltCutDlgMoved()));
}

PhotoShotDialog::~PhotoShotDialog(void)
{

}

void PhotoShotDialog::sltCutDlgMoved()
{
    Q_EMIT signalMoved(dialog->geometry());
}

void PhotoShotDialog::paintEvent(QPaintEvent */*e*/)
{
	QPainterPath painterPath;
	QPainterPath p;
	p.addRect(x(),y(),rect().width(),rect().height());
	painterPath.addRect(dialog->geometry());
    QPainterPath drawPath = p.subtracted(painterPath);

	QPainter paint(this);
	paint.setOpacity(0.7);
	paint.fillPath(drawPath,QBrush(Qt::black));
}
