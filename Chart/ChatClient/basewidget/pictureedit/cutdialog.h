#ifndef CUTDIALOG_H
#define CUTDIALOG_H

#include <QFrame>

#include <QLineF>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

class CutDialog : public QFrame
{
    Q_OBJECT

public:
    explicit CutDialog(QWidget *parent = 0);
    ~CutDialog();
private:
	QRect getResizeGem(QRect oldgeo, QPoint mousePoint);

signals:
	void needMove(int dwidth, int dheight);
    void signalMoved();
protected:
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	QPoint m_startPoint;
	QPoint m_old_pos;
	bool m_isMoving;
	bool m_mouse_down;
	bool m_left;
	bool m_right;
	bool m_bottom;
	bool m_top;
	QLabel *label;
};

#endif // CUTDIALOG_H
