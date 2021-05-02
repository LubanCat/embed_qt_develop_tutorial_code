/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : mainwindow.cpp --- MainWindow
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemagr.h"
#include "myapp.h"
#include "global.h"
#include "unit.h"

#include "tcpserver.h"

#include <QApplication>
#include <QMenu>
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QHostAddress>
#include <QHostInfo>

#include <QMessageBox>
#include <QFileDialog>

#include <QCloseEvent>
#include <QTimerEvent>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    CustomMoveWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_model = new QStandardItemModel(this);

    ui->tableViewUsers->setModel(m_model);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(ui->btnUserInfoPage, 0);
    m_buttonGroup->addButton(ui->btnScorePage, 1);
    m_buttonGroup->addButton(ui->btnScoreInsertPage, 2);
    m_buttonGroup->addButton(ui->btnUsersPage, 3);
    m_buttonGroup->addButton(ui->btnPasswordPage, 4);
    m_buttonGroup->addButton(ui->btnDataBackup, 5);

    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(SltChangePages(int)));
    connect(ui->btnWinClose, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->btnWinMin, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));

    ui->labelWinTitle->setText(tr("局域网通信后台管理平台"));
    ui->stackedWidgetMain->setCurrentIndex(0);
    ui->stackedWidgetFunc->setCurrentIndex(0);

    // 设置几张表格的属性不可编辑
    ui->tableViewUsers->setEditTriggers(QTableView::NoEditTriggers);
    // 设置几张表格的属性选择整行
    ui->tableViewUsers->setSelectionBehavior(QTableView::SelectRows);
    connect(ui->tableViewUsers, SIGNAL(clicked(QModelIndex)), this, SLOT(SltTableClicked(QModelIndex)));

    ui->lineEditBackup->setText(MyApp::m_strBackupPath);

    // 初始化网络
    InitNetwork();

    ui->labelHostAddr->setText(QString("本机IP：") + myHelper::GetIP());
    // 显示系统时间
    ui->labelSystemTime->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ddd"));
    m_nTimerId = startTimer(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
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
 * @brief MainWindow::on_btnLogin_clicked
 */
void MainWindow::on_btnLogin_clicked()
{
    QString name = ui->lineEditUser->text();
    QString passwd = ui->lineEditPasswd->text();
    //
    QJsonObject jsonObje = DataBaseMagr::Instance()->CheckUserLogin(name, passwd);
    int nId = jsonObje.value("id").toInt();
    int identity = jsonObje.value("identity").toInt();

    if (-1 == nId) {
        CMessageBox::Infomation(this, tr("提示"), tr("用户名或密码错误！"));
        return;
    }

    MyApp::m_nId = nId;
    MyApp::m_nIdentyfi = identity;
//    // 设置权限
//    SetUserIdentity(identity);

    ui->stackedWidgetMain->setCurrentIndex(1);
}

/**
 * @brief MainWindow::on_btnExit_clicked
 */
void MainWindow::on_btnExit_clicked()
{
    this->close();
}

/**
 * @brief MainWindow::on_btnQuit_clicked
 * 退出系统，恢复登录界面
 */
void MainWindow::on_btnQuit_clicked()
{
    ui->stackedWidgetMain->setCurrentIndex(0);
}

// 初始化网络
void MainWindow::InitNetwork()
{
    tcpMsgServer = new TcpMsgServer(this);
    bool bOk = tcpMsgServer->StartListen(60100);
    ui->textBrowser->setText(tr("服务器通知消息:"));
    ui->textBrowser->append(bOk ? tr("消息服务器监听成功,端口: 60100") : tr("消息服务器监听失败"));

    tcpFileServer = new TcpFileServer(this);
    bOk = tcpFileServer->StartListen(60101);
    ui->textBrowser->append(bOk ? tr("文件服务器监听成功,端口: 60101") : tr("文件服务器监听失败"));

    systemTrayIcon = new QSystemTrayIcon(this);
    systemTrayIcon->setIcon(QIcon(":/resource/images/ic_app.png"));

    QMenu *m_trayMenu = new QMenu(this);
    m_trayMenu->addAction("显示主面板");
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("退出");

    systemTrayIcon->setContextMenu(m_trayMenu);
    systemTrayIcon->show();

    connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(SltTrayIcoClicked(QSystemTrayIcon::ActivationReason)));
    connect(m_trayMenu, SIGNAL(triggered(QAction*)), this, SLOT(SltTrayIconMenuClicked(QAction*)));

    connect(tcpMsgServer, SIGNAL(signalDownloadFile(QJsonValue)), tcpFileServer, SLOT(SltClientDownloadFile(QJsonValue)));
    connect(tcpMsgServer, SIGNAL(signalUserStatus(QString)), this, SLOT(ShowUserStatus(QString)));
    connect(tcpFileServer, SIGNAL(signalUserStatus(QString)), this, SLOT(ShowUserStatus(QString)));
}

/**
 * @brief MainWindow::SetUserIdentity
 * 根据用户的身份进行界面显示控制
 * @param identity
 */
void MainWindow::SetUserIdentity(const int &identity)
{
    ui->btnScoreInsertPage->setVisible(Mangager == identity);
    ui->btnUsersPage->setVisible(Administrator == identity);
    ui->btnUserInfoPage->setVisible(Administrator == identity);
    ui->btnDataBackup->setVisible(Mangager == identity);

    // 根据用户权限设定界面功能的使用
    if (Mangager == identity) ui->stackedWidgetFunc->setCurrentIndex(1);
    else if (Worker == identity) ui->stackedWidgetFunc->setCurrentIndex(1);
    else if (Administrator == identity) ui->stackedWidgetFunc->setCurrentIndex(0);
}


/**
 * @brief MainWindow::SltChangePages
 * 界面切换
 * @param index
 */
void MainWindow::SltChangePages(int index)
{
    static int s_nIndex = 0;
    if (index == s_nIndex) return;

    ui->stackedWidgetFunc->setLength(ui->stackedWidgetFunc->width(),
                                     index > s_nIndex
                                     ? AnimationStackedWidget::LeftToRight
                                     : AnimationStackedWidget::RightToLeft);

    ui->stackedWidgetFunc->start(index);
    s_nIndex = index;
}


// 备份数据库
void MainWindow::on_btnBackup_clicked()
{
    QString strNewFile = MyApp::m_strBackupPath + QString("info_%1.bak").arg(QDate::currentDate().toString("yyyy_MM_dd"));
    if (QFile::exists(strNewFile)) {
        QFile::remove(strNewFile);
    }
    bool bOk = QFile::copy(MyApp::m_strDatabasePath + "info.db", strNewFile);
    CMessageBox::Infomation(this, bOk ? tr("数据备份成功") : tr("数据备份失败"));
}

// 备份还原
void MainWindow::on_btnDataUdo_clicked()
{
    if (CMessageBox::Accepted == CMessageBox::Question(this, tr("是否还原数据库，该操作不可逆，请确认?")))
    {
        DataBaseMagr::Instance()->CloseDb();
        bool bOk = QFile::remove(MyApp::m_strDatabasePath + "info.db");
        if (bOk) {
            QString strFile = QFileDialog::getOpenFileName(this, tr("选择还原文件"),
                                                           MyApp::m_strBackupPath,
                                                           tr("备份(*.bak)"));
            if (strFile.isEmpty()) {
                CMessageBox::Infomation(this, tr("备份文件选择错误，还原终止！"));
                // 加载数据库
                DataBaseMagr::Instance()->OpenDb(MyApp::m_strDatabasePath + "info.db");
                return;
            }

            bOk = QFile::copy(strFile, MyApp::m_strDatabasePath + "info.db");
            CMessageBox::Infomation(this, bOk ? tr("数据还原成功！") : tr("数据还原失败！"));
        }
        else
        {
            CMessageBox::Infomation(this, tr("提示"), tr("删除文件错误，还原终止！"));
        }

        // 重新打开数据库
        DataBaseMagr::Instance()->OpenDb(MyApp::m_strDatabasePath + "info.db");
    }
}


// 更新消息
void MainWindow::ShowUserStatus(const QString &text)
{
    ui->textBrowser->append(text);
}

/**
 * @brief MainWindow::SltTableClicked
 * 点击
 * @param index
 */
void MainWindow::SltTableClicked(const QModelIndex &index)
{
    qDebug() << index.row();
}

/**
 * @brief MainWindow::SltTrayIcoClicked
 * 托盘菜单
 * @param reason
 */
void MainWindow::SltTrayIcoClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    {
    }
        break;
    case QSystemTrayIcon::DoubleClick:
    {
        if (!this->isVisible())
            this->show();
    }
        break;
    default:
        break;
    }
}

void MainWindow::SltTrayIconMenuClicked(QAction *action)
{
    if ("退出" == action->text()) {
        tcpMsgServer->CloseListen();
        tcpFileServer->CloseListen();
        qApp->quit();
    }
    else if ("显示主面板" == action->text()) {
        this->show();
    }
}

// 获取所有用户
void MainWindow::on_btnUserRefresh_clicked()
{
    QJsonArray jsonArry = DataBaseMagr::Instance()->GetAllUsers();

    m_model->clear();
    m_model->setColumnCount(5);
    m_model->setRowCount(jsonArry.size());
    m_model->setHorizontalHeaderLabels(QStringList() << "用户" << "姓名" << "密码" << "状态" << "最后登录时间");

    for (int i = 0; i < jsonArry.size(); i++) {
        QJsonObject jsonObj = jsonArry.at(i).toObject();

        m_model->setData(m_model->index(i, 0), jsonObj.value("id").toInt());
        m_model->setData(m_model->index(i, 1), jsonObj.value("name").toString());
        m_model->setData(m_model->index(i, 2), jsonObj.value("passwd").toString());
        int nStatus = jsonObj.value("status").toInt();
        m_model->setData(m_model->index(i, 3), OnLine == nStatus ? tr("在线") : tr("离线"));
        m_model->setData(m_model->index(i, 4), jsonObj.value("lasttime").toString());
    }

    ui->tableViewUsers->setColumnWidth(4, 150);
}

/**
 * @brief MainWindow::on_btnUserInsert_clicked
 * 添加用户
 */
void MainWindow::on_btnUserInsert_clicked()
{
    QString strName = ui->lineEditAddUser->text();
    QString strPasswd = ui->lineEditAddPasswd->text();

    if (strName.isEmpty()) {
        CMessageBox::Infomation(this, tr("提示"), tr("用户名不能为空"));
        return;
    }

    strPasswd = strPasswd.isEmpty() ? "123456" : strPasswd;

    int nId = DataBaseMagr::Instance()->RegisterUser(strName, strPasswd);

    // 判断
    CMessageBox::Infomation(this, tr("提示"), -1 == nId ? tr("用户添加失败") : tr("用户添加成功！"));
    if (-1 != nId) {
        on_btnUserRefresh_clicked();
    }
}

/**
 * @brief MainWindow::closeEvent
 * 拦截关闭事件
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event) {
#if 1
    this->hide();
    event->ignore();
#else
    QWidget::closeEvent(event);
#endif
}

/**
 * @brief MainWindow::timerEvent
 * 时间定时器
 * @param e
 */
void MainWindow::timerEvent(QTimerEvent *e)
{
    if (m_nTimerId == e->timerId()) {
        ui->labelSystemTime->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ddd"));
    }
}
