#ifndef SYSTEMMESSAGEDIALOG_H
#define SYSTEMMESSAGEDIALOG_H

#include "customwidget.h"

#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QMutex>

#define ANIMATION_SHOW

namespace Ui {
class SystemMessageDialog;
}

class SystemMessageDialog : public CustomWidget
{
    Q_OBJECT

public:
    explicit SystemMessageDialog(QWidget *parent = 0);
    ~SystemMessageDialog();

#if 1
    static SystemMessageDialog *Instance() {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new SystemMessageDialog();
            }
        }

        return self;
    }
#endif

    void showMessages(const QString &text, const QString &title = "");
    void quitSystem();
private:
    Ui::SystemMessageDialog *ui;
    static SystemMessageDialog *self;

#ifdef ANIMATION_SHOW
    QPropertyAnimation *m_animation;
    QRect hideRect;
    QRect showRect;
#else
    QTimer *m_timerShow;
    QTimer *m_timerStay;

    int m_nTimeCnt;
    int m_nDeskH;
    double transparent;
    QPoint m_pointNomal;
#endif
    QRect m_deskRect;

    QTimer *m_timerClose;
private slots:
    void sltWidgetMove();
    void sltWidgetStay();
    void sltWidgetClose();

protected:
//    void mouseMoveEvent(QMouseEvent *e);
//    void mousePressEvent(QMouseEvent *e);
//    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // SYSTEMMESSAGEDIALOG_H
