/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : loginwidget.cpp --- LoginWidget
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "mainwindow.h"
#include "databasemagr.h"
#include "unit.h"
#include "myapp.h"
#include "global.h"

#include <QDebug>
#include <QMessageBox>
#include <QPainter>

#include <QPropertyAnimation>
#include <QCompleter>
#include <QStringListModel>

LoginWidget::LoginWidget(QWidget *parent) :
    CustomMoveWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);

    m_bConnected = false;

    InitWidget();

    // socket通信
    m_tcpSocket = new ClientSocket();
    ui->labelWinTitle->setText(tr("正在连接服务器..."));
    m_tcpSocket->ConnectToHost(MyApp::m_strHostAddr, MyApp::m_nMsgPort);

    connect(m_tcpSocket, SIGNAL(signalStatus(quint8)), this, SLOT(SltTcpStatus(quint8)));
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * @brief LoginWidget::on_btnLogin_clicked
 * 用户登陆
 */
void LoginWidget::on_btnLogin_clicked()
{
    ui->widgetInput->setVisible(false);

    QPropertyAnimation *animation = new QPropertyAnimation(ui->labelUserHead, "pos");
    animation->setDuration(200);
    animation->setStartValue(QPoint(40, 115));
    animation->setEndValue(QPoint((this->width() - ui->labelUserHead->width()) / 2 - 20, 100));
    connect(animation, SIGNAL(finished()), this, SLOT(SltAnimationFinished()));

    animation->start();
}

///**
// * @brief LoginWidget::on_btnRegedit_clicked
// * 用户注册
// */
//void LoginWidget::on_btnRegedit_clicked()
//{
//    // 如果没有链接上服务器，此时进行一次链接
//    if (!m_bConnected) {
//        m_tcpSocket->ConnectToHost(MyApp::m_strHostAddr, MyApp::m_nMsgPort);
//        CMessageBox::Infomation(this, "未连接服务器，请等待！");
//        return;
//    }

//    // 构建 Json 对象
//    QJsonObject json;
//    json.insert("name", ui->lineEditUser->text());
//    json.insert("passwd", ui->lineEditPasswd->text());

//    m_tcpSocket->SltSendMessage(Register, json);
//}

/**
 * @brief LoginWidget::SltTcpStatus
 * 读取服务器返回的登陆信息状态
 * @param state
 */
void LoginWidget::SltTcpStatus(const quint8 &state)
{
    switch (state) {
    case DisConnectedHost: {
        m_bConnected = false;
        ui->labelWinTitle->setText(tr("服务器已断开"));
    }
        break;
    case ConnectedHost:
    {
        m_bConnected = true;
        ui->labelWinTitle->setText(tr("已连接服务器"));
    }
        break;
        // 登陆验证成功
    case LoginSuccess:
    {
        disconnect(m_tcpSocket, SIGNAL(signalStatus(quint8)), this, SLOT(SltTcpStatus(quint8)));

        // 登录成功后，保存当前用户
        MyApp::m_strUserName = ui->lineEditUser->text();
        MyApp::m_strPassword = ui->lineEditPasswd->text();
        MyApp::SaveConfig();

        // 显示主界面
        MainWindow *mainWindow = new MainWindow();
        if (!QFile::exists(MyApp::m_strHeadFile)) {
            myHelper::Sleep(100);
            QJsonObject jsonObj;
            jsonObj.insert("from", MyApp::m_nId);
            jsonObj.insert("id",  -2);
            jsonObj.insert("msg", myHelper::GetFileNameWithExtension(MyApp::m_strHeadFile));
            m_tcpSocket->SltSendMessage(GetFile, jsonObj);
            myHelper::Sleep(100);
        }

        // 居中显示
        myHelper::FormInCenter(mainWindow);
        mainWindow->SetSocket(m_tcpSocket, MyApp::m_strUserName);
        mainWindow->show();
        this->close();
    }
        break;
    case LoginPasswdError:
    {
        CMessageBox::Infomation(this, "登录失败，请检查用户名和密码！");
    }
        break;
    case LoginRepeat:
    {
        CMessageBox::Infomation(this, "登录失败，该账户已登录！");
    }
        break;

    case RegisterOk:
    {
        CMessageBox::Infomation(this, "该账号注册成功！请点击登录！");
    }
        break;
    case RegisterFailed:
    {
        CMessageBox::Infomation(this, "该账号已经注册！请点击登录！");
    }
        break;
    default:
        break;
    }

    // 还原初始位置，重新登录
    ui->widgetInput->setVisible(true);
    ui->labelUserHead->move(40, 115);
}

/**
 * @brief LoginWidget::on_btnCancel_clicked
 * 取消配置
 */
void LoginWidget::on_btnCancel_clicked()
{
    ui->stackedWidget->nextPage();
}

/**
 * @brief LoginWidget::on_btnSaveCfg_clicked
 * 保存配置
 */
void LoginWidget::on_btnSaveCfg_clicked()
{
    QString strHost = ui->lineEditHostAddr->text();

    // 判断是否ip地址
    if (!myHelper::IsIP(strHost))
    {
        CMessageBox::Infomation(this, tr("IP地址设置有误!"));
        return;
    }

    int nMsgPort = ui->lineEditHostMsgPort->text().toInt();
    if (nMsgPort > 65535 || nMsgPort < 100) {
        CMessageBox::Infomation(this, tr("端口设置有误!"));
        return;
    }

    int nFilePort = ui->lineEditHostFilePort->text().toInt();
    if (nFilePort > 65535 || nFilePort < 100) {
        CMessageBox::Infomation(this, tr("端口设置有误!"));
        return;
    }

    if (nMsgPort == nFilePort) {
        CMessageBox::Infomation(this, tr("2个端口不能设置一样!"));
        return;
    }

    MyApp::m_strHostAddr = strHost;
    MyApp::m_nMsgPort    = nMsgPort;
    MyApp::m_nFilePort   = nFilePort;

    // 保存配置
    MyApp::SaveConfig();

    // 如果没连接服务器，则尝试新的配置连接
    if (!m_bConnected) {
        m_tcpSocket->ConnectToHost(MyApp::m_strHostAddr, MyApp::m_nMsgPort);
    }

    // 返回登录界面
    ui->stackedWidget->nextPage();
}

// 切换配置
void LoginWidget::on_btnWinMenu_clicked()
{
    ui->stackedWidget->nextPage();
}

void LoginWidget::SltAnimationFinished()
{
    if (!m_bConnected) {
        m_tcpSocket->ConnectToHost(MyApp::m_strHostAddr, MyApp::m_nMsgPort);
        CMessageBox::Infomation(this, "未连接服务器，请等待！");
        ui->widgetInput->setVisible(true);
        ui->labelUserHead->move(40, 115);
        return;
    }

    // 构建 Json 对象
    QJsonObject json;
    json.insert("name", ui->lineEditUser->text());
    json.insert("passwd", ui->lineEditPasswd->text());

    m_tcpSocket->SltSendMessage(Login, json);
}

/**
 * @brief LoginWidget::SltEditFinished
 * 编辑完成
 */
void LoginWidget::SltEditFinished()
{
    QString text = ui->lineEditUser->text();
    if (QString::compare(text, QString("")) != 0) {
//        bool flag = valueList.contains(text, Qt::CaseInsensitive);
//        if (!flag) {
////            ui->lineEditUser->setText(text);
//        }
        qDebug() << "text edit finished" << text;
    }
}

/**
 * @brief LoginWidget::InitWidget
 * 界面初始化
 */
void LoginWidget::InitWidget()
{
    // 加载配置信息
    ui->lineEditUser->SetIcon(QPixmap(":/resource/common/ic_user.png"));
    ui->lineEditUser->setText(MyApp::m_strUserName);

    ui->lineEditPasswd->SetIcon(QPixmap(":/resource/common/ic_lock.png"));    // 加载之前的配置

    ui->lineEditHostAddr->setText(MyApp::m_strHostAddr);
    ui->lineEditHostMsgPort->setText(QString::number(MyApp::m_nMsgPort));
    ui->lineEditHostFilePort->setText(QString::number(MyApp::m_nFilePort));
    if (ui->checkBoxPasswd->isChecked())
    {
        ui->lineEditPasswd->setText(MyApp::m_strPassword);
    }

    QStringList valueList;
    valueList << "xiaoni" << "zhangsan" << "dengmc" << "wangwu";

    QCompleter *completer = new QCompleter(valueList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditUser->setCompleter(completer);

    // 信号槽
    connect(ui->lineEditUser, SIGNAL(editingFinished()), this, SLOT(SltEditFinished()));
    connect(ui->lineEditUser, SIGNAL(returnPressed()), this, SLOT(on_btnLogin_clicked()));
    connect(ui->lineEditPasswd, SIGNAL(returnPressed()), this, SLOT(on_btnLogin_clicked()));

    // 默认显示第一界面
    ui->stackedWidget->setCurrentIndex(0);
}

void LoginWidget::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.fillRect(this->rect(), Qt::transparent);
}

// 记住密码
void LoginWidget::on_checkBoxPasswd_clicked(bool checked)
{
    if (!checked) ui->checkBoxAutoLogin->setChecked(false);
}

// 自动登陆
void LoginWidget::on_checkBoxAutoLogin_clicked(bool checked)
{
    if (checked) ui->checkBoxPasswd->setChecked(true);
}
