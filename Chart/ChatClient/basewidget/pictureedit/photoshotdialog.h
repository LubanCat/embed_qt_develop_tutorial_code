#pragma once
#include <QWidget>
#include "cutdialog.h"

class CutDialog;

class PhotoShotDialog :public QWidget
{
	Q_OBJECT

public:
    explicit PhotoShotDialog(QWidget *parent = 0);

    ~PhotoShotDialog(void);

    QRect getShotGeometry(){return dialog->geometry();}

signals:
  void signalMoved(QRect rect);

private:
	CutDialog *dialog;

private slots:
    void sltCutDlgMoved();
protected:
	void paintEvent(QPaintEvent *e);
};

