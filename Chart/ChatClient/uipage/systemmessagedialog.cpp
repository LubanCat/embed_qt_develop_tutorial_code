#include "systemmessagedialog.h"
#include "ui_systemmessagedialog.h"

#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QDateTime>
#include <QMouseEvent>

#define FIX_WIDTH   327
#define FIX_HEIGHT  180

#if 1
SystemMessageDialog *SystemMessageDialog::self = NULL;
#endif

SystemMessageDialog::SystemMessageDialog(QWidget *parent) :
    CustomWidget(parent),
    ui(new Ui::SystemMessageDialog)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(FIX_WIDTH, FIX_HEIGHT));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    //    ui->btnClose->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
//    ui->labelMainIcon->setPixmap(QPixmap(":/res/images/cyhc-30.png").scaled(QSize(36, 22)));

#ifdef ANIMATION_SHOW
    m_animation = new QPropertyAnimation(this, "geometry");
//    m_animation->setEasingCurve(QEasingCurve::OutBounce);
    m_deskRect = QApplication::desktop()->availableGeometry();

    hideRect = QRect(m_deskRect.width() - FIX_WIDTH, m_deskRect.height() + 100, FIX_WIDTH, FIX_HEIGHT);
    showRect = QRect(m_deskRect.width() - FIX_WIDTH, m_deskRect.height() - FIX_HEIGHT, FIX_WIDTH, FIX_HEIGHT);

    m_timerClose = new QTimer(this);
    m_timerClose->setInterval(3000);

    connect(m_timerClose, SIGNAL(timeout()), this, SLOT(sltWidgetClose()));
    connect(ui->btnClose, SIGNAL(clicked(bool)), this, SLOT(sltWidgetClose()));
#else
    m_nTimeCnt = 0;
    transparent = 1.0;
    m_nDeskH = QApplication::desktop()->height();

    m_timerShow = new QTimer(this);
    m_timerStay = new QTimer(this);

    connect(m_timerShow, SIGNAL(timeout()), this, SLOT(sltWidgetMove()));
    connect(m_timerStay, SIGNAL(timeout()), this, SLOT(sltWidgetStay()));
#endif
}

SystemMessageDialog::~SystemMessageDialog()
{
    delete ui;
    if (QPropertyAnimation::Running == m_animation->state()) {
        m_animation->stop();
    }
    delete m_animation;
    m_animation = NULL;   
}

void SystemMessageDialog::quitSystem()
{
    if (QPropertyAnimation::Running == m_animation->state()) {
        m_animation->stop();
    }
    delete m_animation;
    m_animation = NULL;

    this->close();
}

void SystemMessageDialog::showMessages(const QString &text, const QString &title)
{
    if (text.isEmpty()) return;
    QString dateTime = QDateTime::currentDateTime().toString("yyyy年MM月dd日 ddd hh:ss");

    ui->labelDateTime->setText(QString("今天是: %1").arg(dateTime));
    ui->labelContent->setText(text);

    if (!title.isEmpty()) {
        ui->labelTitle->setText(title);
    }

    this->showNormal();

#ifdef ANIMATION_SHOW
    if (QPropertyAnimation::Running == m_animation->state()) {
        m_animation->stop();
    }

    disconnect(m_animation, SIGNAL(finished()), this, SLOT(hide()));
    m_animation->setDuration(1000);
    m_animation->setStartValue(hideRect);
    m_animation->setEndValue(showRect);
    m_animation->start();

    connect(m_animation, SIGNAL(finished()), this, SLOT(sltWidgetStay()));
#else
    m_nTimeCnt = 0;
    transparent = 1.0;
    m_nDeskH = QApplication::desktop()->height();

    m_timerShow->stop();
    m_timerStay->stop();
    m_timerClose->stop();

    setWindowOpacity(1);

    QDesktopWidget *desktop = QApplication::desktop();
    m_deskRect = desktop->availableGeometry();
    m_pointNomal.setX(m_deskRect.width() - rect().width());
    m_pointNomal.setY(m_deskRect.height() - rect().height());
    move(m_pointNomal.x(), m_pointNomal.y());

    m_timerShow->start(5);
#endif
}


void SystemMessageDialog::sltWidgetMove()
{
#ifndef ANIMATION_SHOW
    m_nDeskH--;
    this->move(m_pointNomal.x(), m_nDeskH);

    if(m_nDeskH <= m_pointNomal.y())
    {
        m_timerShow->stop();
        m_timerStay->start(1000);
    }
#endif
}

void SystemMessageDialog::sltWidgetStay()
{
#ifdef ANIMATION_SHOW
    m_timerClose->start();
#else
    m_nTimeCnt++;
    if(m_nTimeCnt >= 9)
    {
        m_timerStay->stop();
        m_timerClose->start(200);
    }
#endif
}

void SystemMessageDialog::sltWidgetClose()
{
#ifdef ANIMATION_SHOW

    m_timerClose->stop();
    disconnect(m_animation, SIGNAL(finished()), this, SLOT(sltWidgetStay()));

    m_animation->stop();
    m_animation->setDuration(1000);
    m_animation->setStartValue(showRect);
    m_animation->setEndValue(hideRect);
    m_animation->start();

    connect(m_animation, SIGNAL(finished()), this, SLOT(hide()));
#else
    transparent -= 0.1;
    if(transparent <= 0.0)
    {
        m_timerClose->stop();
        this->close();
    } else {
        setWindowOpacity(transparent);
    }
#endif
}

//void SystemMessageDialog::mouseMoveEvent(QMouseEvent *e)
//{
//    e->accept();
//}

//void SystemMessageDialog::mousePressEvent(QMouseEvent *e)
//{
//    e->accept();
//}

//void SystemMessageDialog::mouseReleaseEvent(QMouseEvent *e)
//{
//    e->accept();
//}
