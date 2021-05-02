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

#include "unit.h"
#include "qqcell.h"
#include "qqlistviewgroup.h"
#include "global.h"

//#include "testmedia.h"

#include "chatbubble.h"
#include "chatwindow.h"
#include "databasemagr.h"
#include "systemsetting.h"

#include "systemmessagedialog.h"

#include <QTimer>
#include <QMenu>
#include <QInputDialog>
#include <QDebug>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>

#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    CustomMoveWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    m_bQuit = false;

    m_btnGroup = new QButtonGroup(this);
    m_btnGroup->addButton(ui->btnFrind, 0);
    m_btnGroup->addButton(ui->btnGroup, 1);
    m_btnGroup->addButton(ui->btnConversation, 2);
    m_btnGroup->addButton(ui->btnApplay, 3);

    connect(m_btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(SltMainPageChanged(int)));


    // 系统菜单初始化
    InitSysMenu();
    InitQQListMenu();
    InitSysTrayIcon();

    // 默认显示主的界面
    ui->GCStackedWidget->setCurrentIndex(0);

    // 关联头像编辑信号槽
    connect(ui->widgetHead, SIGNAL(signalCutHeadOk()), this, SLOT(SltHeadPicCutOk()));
    connect(ui->widgetHead, SIGNAL(signalUpdateUserHead(int,QString)), this, SLOT(SltUpdateUserHead(int,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug() << "delete";
    delete m_tcpSocket;
    m_tcpSocket = NULL;

    // 关闭所有的聊天窗口
    foreach (ChatWindow *window, m_chatFriendWindows) {
        window->close();
    }

    foreach (ChatWindow *window, m_chatGroupWindows) {
        window->close();
    }
}

/**
 * @brief MainWindow::SetSocket
 * @param tcpSocket
 * @param name
 */
void MainWindow::SetSocket(ClientSocket *tcpSocket, const QString &name)
{
    if (NULL != tcpSocket) {
        m_tcpSocket = tcpSocket;

        connect(m_tcpSocket, SIGNAL(signalMessage(quint8,QJsonValue)), this, SLOT(SltTcpReply(quint8,QJsonValue)));
        connect(m_tcpSocket, SIGNAL(signalStatus(quint8)), this, SLOT(SltTcpStatus(quint8)));

        // 加载头像
        ui->widgetHead->SetHeadPixmap(MyApp::m_strHeadFile);

        // 上线的时候获取当前好友的状态
        QJsonArray friendArr = DataBaseMagr::Instance()->GetMyFriend(MyApp::m_nId);

        // 组织Jsonarror
        m_tcpSocket->SltSendMessage(GetMyFriends, friendArr);

        // 添加我的群组
        AddMyGroups(DataBaseMagr::Instance()->GetMyGroup(MyApp::m_nId));
    }

    if (name.isEmpty()) return;
    ui->labelUser->setText(name);
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
 * @brief MainWindow::closeEvent
 * 拦截关闭事件
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event) {
#if 0
    this->hide();
    event->ignore();
#else
    event->accept();
    QWidget::closeEvent(event);
#endif
}

void MainWindow::showEvent(QShowEvent *event)
{
    MyApp::m_nWinX = this->pos().x();
    MyApp::m_nWinY = this->pos().y();
    QWidget::showEvent(event);
}
/**
 * @brief MainWindow::InitMenu
 */
void MainWindow::InitSysMenu()
{
    // 设置子菜单
    QMenu *sysmenu = new QMenu(this);
    sysmenu->addAction(tr("系统设置"));
    sysmenu->addAction("消息管理器");
    sysmenu->addAction("文件助手");
    sysmenu->addSeparator();
    sysmenu->addAction("修改密码");
    sysmenu->addAction("帮助");
    sysmenu->addAction("连接服务器");
    sysmenu->addSeparator();
    sysmenu->addAction("升级");

    // 添加菜单
    ui->btnSysMenu->setMenu(sysmenu);
    connect(sysmenu, SIGNAL(triggered(QAction*)), this, SLOT(SltSysmenuCliecked(QAction*)));
}

/**
 * @brief MainWindow::InitQQListMenu
 * 添加好友列表菜单
 */
void MainWindow::InitQQListMenu()
{
    //设置菜单
    //设置组菜单
    QMenu *addFriend = new QMenu(this);
    addFriend->addAction(tr("添加好友"));
    addFriend->addAction(tr("刷新"));
    addFriend->addSeparator();
    addFriend->addAction(tr("删除该组"));
    connect(addFriend, SIGNAL(triggered(QAction*)), this, SLOT(onAddFriendMenuDidSelected(QAction*)));
    ui->frindListWidget->setGroupPopMenu(addFriend);

    // 设置子菜单
    QMenu *childMenu = new QMenu(this);
    childMenu->addAction(tr("发送即时消息"));
    childMenu->addSeparator();
    childMenu->addAction("移动至黑名单");
    childMenu->addAction("删除联系人");

    QMenu *groupListMenu = new QMenu(tr("移动联系人至"));
    childMenu->addMenu(groupListMenu);

    // childMenu->addAction(tr("创建讨论组"));
    connect(childMenu, SIGNAL(triggered(QAction*)), this, SLOT(onChildPopMenuDidSelected(QAction*)));
    ui->frindListWidget->setChildPopMenu(childMenu);

    //添加默认项
    //好友列表
    QQCell *myFriend = new QQCell;
    myFriend->groupName = QString(tr("我的好友"));
    myFriend->isOpen = false;
    myFriend->type = QQCellType_Group;
    myFriend->name = QString(tr("我的好友"));
    myFriend->subTitle = QString("(0/0)");
    ui->frindListWidget->insertQQCell(myFriend);

    QQCell *blacklist = new QQCell;
    blacklist->groupName = QString(tr("黑名单"));
    blacklist->isOpen = false;
    blacklist->type = QQCellType_Group;
    blacklist->name = QString(tr("黑名单"));
    blacklist->subTitle = QString("(0/0)");
    ui->frindListWidget->insertQQCell(blacklist);

    connect(ui->frindListWidget, SIGNAL(onChildDidDoubleClicked(QQCell*)), this, SLOT(SltFriendsClicked(QQCell*)));

    //组列表
    QMenu *myGroupMenu = new QMenu(this);
    myGroupMenu->addAction(tr("创建讨论组"));
    myGroupMenu->addAction(tr("加入讨论组"));
    myGroupMenu->addAction(tr("删除该组"));
    myGroupMenu->addSeparator();
    connect(myGroupMenu, SIGNAL(triggered(QAction*)), this, SLOT(onGroupPopMenuDidSelected(QAction*)));
    ui->groupListWidget->setGroupPopMenu(myGroupMenu);
    //设置子菜单
    QMenu *groupChildMen = new QMenu(this);
    groupChildMen->addAction(tr("发送即时消息"));
    ui->groupListWidget->setChildPopMenu(groupChildMen);

    //添加默认项
    QQCell *groupCell = new QQCell;
    groupCell->groupName = QString(tr("我的群组"));
    groupCell->isOpen = false;
    groupCell->type = QQCellType_GroupEx;
    groupCell->name = QString(tr("讨论组"));
    groupCell->subTitle = QString("(0/0)");
    ui->groupListWidget->insertQQCell(groupCell);

    connect(ui->groupListWidget, SIGNAL(onChildDidDoubleClicked(QQCell*)), this, SLOT(SltGroupsClicked(QQCell*)));
}

/**
 * @brief MainWindow::InitSysTrayIcon
 * 托盘菜单
 */
void MainWindow::InitSysTrayIcon()
{
    systemTrayIcon = new QSystemTrayIcon(this);
    systemTrayIcon->setIcon(QIcon(":/resource/background/app.png"));

    QMenu *m_trayMenu = new QMenu(this);
    m_trayMenu->addAction("我在线上");
    m_trayMenu->addAction("离线");
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("显示主面板");
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("退出");

    systemTrayIcon->setContextMenu(m_trayMenu);
    systemTrayIcon->show();

    connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(SltTrayIcoClicked(QSystemTrayIcon::ActivationReason)));
    connect(m_trayMenu, SIGNAL(triggered(QAction*)), this, SLOT(SltTrayIconMenuClicked(QAction*)));
}

/**
 * @brief MainWindow::SltReadMessages
 * 读取消息
 * @param txt
 * @param ip
 */
void MainWindow::SltReadMessages(const QJsonValue &json, const int &id)
{
    // 如果收到消息时有聊天窗口存在，直接添加到聊天记录，并弹出窗口
    foreach (ChatWindow *window, m_chatFriendWindows) {
        if (window->GetUserId() == id) {
            window->AddMessage(json);
            window->show();
            return;
        }
    }

    // 没有检索到聊天窗口，直接弹出新窗口
    QList<QQCell *> groups = ui->frindListWidget->getCells();
    foreach (QQCell *cell, groups.at(0)->childs) {
        if (cell->id == id) {
            ChatWindow *chatWindow = new ChatWindow();
            connect(chatWindow, SIGNAL(signalSendMessage(quint8,QJsonValue)), m_tcpSocket, SLOT(SltSendMessage(quint8,QJsonValue)));
            connect(chatWindow, SIGNAL(signalClose()), this, SLOT(SltFriendChatWindowClose()));

            chatWindow->SetCell(cell);
            chatWindow->AddMessage(json);
            chatWindow->show();
            // 添加到当前聊天框
            m_chatFriendWindows.append(chatWindow);
            return;
        }
    }
}

/**
 * @brief MainWindow::SltMainPageChanged
 * @param index
 */
void MainWindow::SltMainPageChanged(int index)
{
    static int s_nIndex = 0;
    if (index == s_nIndex) return;

    ui->GCStackedWidget->setLength(ui->GCStackedWidget->width(),
                                   index > s_nIndex
                                   ? AnimationStackedWidget::LeftToRight
                                   : AnimationStackedWidget::RightToLeft);

    ui->GCStackedWidget->start(index);
    s_nIndex = index;
}

/**
 * @brief MainWindow::SltFriendsClicked
 * @param cell
 */
void MainWindow::SltFriendsClicked(QQCell *cell)
{
    // 判断与该用户是否有聊天窗口，如果有弹出窗口
    foreach (ChatWindow *window, m_chatFriendWindows) {
        if (window->GetUserId() == cell->id) {
            window->show();
            return;
        }
    }

    // 没有检索到聊天窗口，直接弹出新窗口
    ChatWindow *chatWindow = new ChatWindow();
    connect(chatWindow, SIGNAL(signalSendMessage(quint8,QJsonValue)), m_tcpSocket, SLOT(SltSendMessage(quint8,QJsonValue)));
    connect(chatWindow, SIGNAL(signalClose()), this, SLOT(SltFriendChatWindowClose()));
    // 设置窗口属性
    chatWindow->SetCell(cell);
    chatWindow->show();

    // 添加到当前聊天框
    m_chatFriendWindows.append(chatWindow);
}

/**
 * @brief MainWindow::SltGroupsClicked
 * 群组被双击打开
 * @param cell
 */
void MainWindow::SltGroupsClicked(QQCell *cell)
{
    // 构建 Json 对象
    QJsonObject json;
    json.insert("id", cell->id);
    json.insert("name", cell->name);

    m_tcpSocket->SltSendMessage(GetMyGroups, json);

    // 判断与该用户是否有聊天窗口，如果有弹出窗口
    foreach (ChatWindow *window, m_chatGroupWindows) {
        if (window->GetUserId() == cell->id) {
            window->show();
            return;
        }
    }

    // 没有检索到聊天窗口，直接弹出新窗口
    ChatWindow *chatWindow = new ChatWindow();
    connect(chatWindow, SIGNAL(signalSendMessage(quint8,QJsonValue)), m_tcpSocket, SLOT(SltSendMessage(quint8,QJsonValue)));
    connect(chatWindow, SIGNAL(signalClose()), this, SLOT(SltGroupChatWindowClose()));

    chatWindow->SetCell(cell, 1);
    chatWindow->show();

    // 添加到当前聊天框
    m_chatGroupWindows.append(chatWindow);
}

/**
 * @brief MainWindow::onAddFriendMenuDidSelected
 * 添加好友
 * @param action
 */
void MainWindow::onAddFriendMenuDidSelected(QAction *action)
{
    if (!action->text().compare(tr("添加好友")))
    {
        QString text = CInputDialog::GetInputText(this, "zhangsan");

        if (!text.isEmpty()) {
            // 首先判断该用户是否已经是我的好友了
            if (DataBaseMagr::Instance()->isMyFriend(MyApp::m_nId, text)) {
                CMessageBox::Infomation(this, "该用户已经是你的好友了！");
                return;
            }

            // 构建 Json 对象
            QJsonObject json;
            json.insert("id", m_tcpSocket->GetUserId());
            json.insert("name", text);

            m_tcpSocket->SltSendMessage(AddFriend, json);
        }
    }
    else if (!action->text().compare(tr("刷新")))
    {
        // 上线的时候获取当前好友的状态
        QJsonArray friendArr = DataBaseMagr::Instance()->GetMyFriend(MyApp::m_nId);

        // 组织Jsonarror
        m_tcpSocket->SltSendMessage(RefreshFriends, friendArr);
    }
    else if (!action->text().compare(tr("删除该组")))
    {
        qDebug() << "delete group";
    }
}

void MainWindow::onGroupPopMenuDidSelected(QAction *action)
{
    if (!action->text().compare(tr("创建讨论组")))
    {
        QString text = CInputDialog::GetInputText(this, "我的朋友们");
        if (!text.isEmpty()) {

            // 构建 Json 对象
            QJsonObject json;
            json.insert("id", m_tcpSocket->GetUserId());
            json.insert("name", text);

            m_tcpSocket->SltSendMessage(CreateGroup, json);
        }
    }
    else if (!action->text().compare(tr("加入讨论组")))
    {
        QString text = CInputDialog::GetInputText(this, "我的朋友们");
        if (!text.isEmpty()) {
            // 首先判断是否已经添加该群组了
            if (DataBaseMagr::Instance()->isInGroup(text)) {
                CMessageBox::Infomation(this, "你已经添加该群组了！");
                return;
            }

            // 构建 Json 对象
            QJsonObject json;
            json.insert("id", MyApp::m_nId);
            json.insert("name", text);

            m_tcpSocket->SltSendMessage(AddGroup, json);
        }
    }
    else if (!action->text().compare(tr("删除该组")))
    {
        qDebug() << "delete group";
    }
}

void MainWindow::onChildPopMenuDidSelected(QAction *action)
{
    QQCell *cell = ui->frindListWidget->GetRightClickedCell();
    if (NULL == cell) return;

    if (!action->text().compare(tr("发送即时消息")))
    {
        qDebug() << "send message" << cell->name;
        // 打开对话框
        SltFriendsClicked(cell);
    }
    else if (!action->text().compare(tr("移动至黑名单")))
    {
        qDebug() << "balck friend list" << cell->name;
    }
    else if (!action->text().compare(tr("删除联系人")))
    {
        qDebug() << "delete my friend" << cell->name;
        ui->frindListWidget->removeQQCell(cell);
        // 更新数据库
        bool bOk = DataBaseMagr::Instance()->DeleteMyFriend(cell->id, MyApp::m_nId);
        CMessageBox::Infomation(this, bOk ? tr("用户删除成功！") : tr("用户删除失败！"));
    }
}

/**
 * @brief MainWindow::SltChatWindowClose
 * 聊天窗口主动关掉
 * @param ip
 */
void MainWindow::SltFriendChatWindowClose()
{
    ChatWindow *chatWindow = (ChatWindow*)sender();
    disconnect(chatWindow, SIGNAL(signalSendMessage(quint8,QJsonValue)), m_tcpSocket, SLOT(SltSendMessage(quint8,QJsonValue)));
    disconnect(chatWindow, SIGNAL(signalClose()), this, SLOT(SltFriendChatWindowClose()));

    if (!this->isVisible() && m_chatFriendWindows.size() == 1)
    {
        this->show();
    }

    // 删除聊天窗口
    m_chatFriendWindows.removeOne(chatWindow);
}

/**
 * @brief MainWindow::SltGroupChatWindowClose
 * 群聊窗口关闭
 */
void MainWindow::SltGroupChatWindowClose()
{
    ChatWindow *chatWindow = (ChatWindow*)sender();
    disconnect(chatWindow, SIGNAL(signalSendMessage(quint8,QJsonValue)), m_tcpSocket, SLOT(SltSendMessage(quint8,QJsonValue)));
    disconnect(chatWindow, SIGNAL(signalClose()), this, SLOT(SltGroupChatWindowClose()));
    if (!this->isVisible() && m_chatGroupWindows.size() == 1)
    {
        this->show();
    }
    // 删除聊天窗口
    m_chatGroupWindows.removeOne(chatWindow);
}

/**
 * @brief MainWindow::SltTcpStatus
 * tcp状态监听
 * @param state
 */
void MainWindow::SltTcpStatus(const quint8 &state)
{
    switch (state) {
    case DisConnectedHost: {
        if (m_bQuit) return;
        QPixmap pixmap  = QPixmap(":/resource/background/app.png");
        pixmap = myHelper::ChangeGrayPixmap(pixmap.toImage());
        systemTrayIcon->setIcon(QIcon(pixmap));
        pixmap = QPixmap(MyApp::m_strHeadFile);
        ui->widgetHead->SetHeadPixmap(myHelper::ChangeGrayPixmap(pixmap.toImage()));
        if (!this->isVisible()) this->show();
        CMessageBox::Infomation(this, tr("与服务器断开连接"));
    }
        break;
    case ConnectedHost:
    {
        QJsonObject json;
        json.insert("name", MyApp::m_strUserName);
        json.insert("passwd", MyApp::m_strPassword);

        m_tcpSocket->SltSendMessage(Login, json);
    }
        break;
    case LoginSuccess:
    {
        systemTrayIcon->setIcon(QIcon(":/resource/background/app.png"));
        ui->widgetHead->SetHeadPixmap(QPixmap(MyApp::m_strHeadFile));
        if (!this->isVisible()) this->show();
        CMessageBox::Infomation(this, tr("重连成功！"));
    }
        break;
    default:
        break;
    }
}

/**
 * @brief MainWindow::SltTcpReply
 * @param type
 * @param msg
 */
void MainWindow::SltTcpReply(const quint8 &type, const QJsonValue &dataVal)
{
    switch (type) {
    case UserOnLine:
    {
        UpdateFriendStatus(OnLine, dataVal);
    }
        break;
    case UserOffLine:
    {
        UpdateFriendStatus(OffLine, dataVal);
    }
        break;
    case UpdateHeadPic:
    {
        // 你的好友更新了头像
        ParseUpFriendHead(dataVal);
    }
        break;
    case AddFriend:
    {
        ParseAddFriendReply(dataVal);
    }
        break;
    case AddGroup:
    {
        ParseAddGroupReply(dataVal);
    }
        break;

    case AddFriendRequist:
    {
        ParseAddFriendRequest(dataVal);
    }
        break;
    case AddGroupRequist:
    {
        ParseAddGroupRequest(dataVal);
    }
        break;

    case CreateGroup:
    {
        ParseCreateGroupReply(dataVal);
    }
        break;
    case GetMyFriends:
    {
        ParseGetFriendsReply(dataVal);
    }
        break;
    case GetMyGroups:
    {
        ParseGetGroupFriendsReply(dataVal);
    }
        break;
    case RefreshFriends:
    {
        ParseRefreshFriendsReply(dataVal);
    }
        break;
    case RefreshGroups:
    {
        ParseRefreshGroupFriendsReply(dataVal);
    }
        break;
    case SendMsg:
    {
        ParseFriendMessageReply(dataVal);
    }
        break;
    case SendGroupMsg:
    {
        ParseGroupMessageReply(dataVal);
    }
        break;
    case SendFile:
    {
        ParseFriendMessageReply(dataVal);
    }
        break;
    case SendPicture:
    {
        ParseFriendMessageReply(dataVal);
    }
        break;
    default:
        break;
    }
}


/**
 * @brief TcpClient::ParseAddFriendReply
 * @param message
 */
void MainWindow::ParseAddFriendReply(const QJsonValue &dataVal)
{
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();

        int nStatus = dataObj.value("status").toInt();

        int nId = dataObj.value("id").toInt();
        // 未查询到该用户
        if (-1 == nId) {
            CMessageBox::Infomation(this, "未找到该用户!");
            return;
        }

        QString strHead = dataObj.value("head").toString();
        // 头像判断，如果不在就申请
        if (!QFile::exists(MyApp::m_strHeadPath + strHead))
        {
            QJsonObject jsonObj;
            jsonObj.insert("from", MyApp::m_nId);
            jsonObj.insert("id",  -2);
            jsonObj.insert("msg", strHead);
            m_tcpSocket->SltSendMessage(GetFile, jsonObj);

            myHelper::Sleep(100);
        }

        QQCell *cell = new QQCell;
        cell->groupName = QString(tr("我的好友"));
        cell->iconPath  = GetHeadPixmap(strHead);
        cell->type      = QQCellType_Child;
        cell->name      = dataObj.value("name").toString();
        cell->subTitle  = QString("当前用户状态：%1 ").arg(OnLine == nStatus ? tr("在线") : tr("离线"));
        cell->id        = nId;
        cell->status    = nStatus;

        ui->frindListWidget->insertQQCell(cell);

        // 更新至数据库
        DataBaseMagr::Instance()->AddFriend(cell->id, MyApp::m_nId, cell->name);
    }
}

/**
 * @brief MainWindow::ParseAddGroupReply
 * @param dataVal
 */
void MainWindow::ParseAddGroupReply(const QJsonValue &dataVal)
{
    if (dataVal.isObject()) {
        QJsonObject jsonObj = dataVal.toObject();
        int nId = jsonObj.value("id").toInt();
        if (-1 == nId) {
            CMessageBox::Infomation(this, "未找到该群组!");
            return;
        }

        QQCell *cell = new QQCell;
        cell->groupName = QString(tr("我的群组"));
        cell->iconPath  = GetHeadPixmap(jsonObj.value("head").toString());
        cell->type      = QQCellType_Child;
        cell->name      = jsonObj.value("name").toString();
        cell->subTitle  = QString("我的群，我做主...");
        cell->id        = nId;
        cell->status    = OnLine;

        ui->groupListWidget->insertQQCell(cell);

        // 更新至数据库
        DataBaseMagr::Instance()->AddGroup(nId, MyApp::m_nId, cell->name);
    }
}

/**
 * @brief MainWindow::ParseCreateGroupReply
 * @param dataVal
 */
void MainWindow::ParseCreateGroupReply(const QJsonValue &dataVal)
{
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();

        int nId = dataObj.value("id").toInt();
        // 未查询到该用户
        if (-1 == nId) {
            CMessageBox::Infomation(this, "该群组已经添加!");
            return;
        }

        QQCell *cell = new QQCell;
        cell->groupName = QString(tr("我的群组"));
        cell->iconPath  = GetHeadPixmap(dataObj.value("head").toString());
        cell->type      = QQCellType_Child;
        cell->name      = dataObj.value("name").toString();
        cell->subTitle  = QString("我的群，我做主...");
        cell->id        = nId;
        cell->status    = OnLine;

        ui->groupListWidget->insertQQCell(cell);

        // 更新至数据库
        DataBaseMagr::Instance()->AddGroup(cell->id, MyApp::m_nId, cell->name);
    }
}

/**
 * @brief MainWindow::ParseAddFriendRequest
 * 被别人添加为好友
 * @param dataVal
 */
void MainWindow::ParseAddFriendRequest(const QJsonValue &dataVal)
{
    //
    if(dataVal.isObject()) {
        QJsonObject jsonObj = dataVal.toObject();
        int nId = jsonObj.value("id").toInt();
        QString name = jsonObj.value("name").toString();
        QString strMsg = jsonObj.value("msg").toString();

        // 判断我是否已经添加该好友
        if (!DataBaseMagr::Instance()->isMyFriend(MyApp::m_nId, name)) {
            QQCell *cell = new QQCell;
            cell->groupName = QString(tr("我的好友"));
            cell->iconPath  = MyApp::m_strHeadPath + jsonObj.value("head").toString();
            cell->type      = QQCellType_Child;
            cell->name     = name;
            cell->subTitle  = QString("当前用户状态：在线 ");
            cell->id        = nId;
            cell->status    = OnLine;

            ui->frindListWidget->insertQQCell(cell);

            // 更新至数据库
            DataBaseMagr::Instance()->AddFriend(cell->id, MyApp::m_nId, cell->name);
        }

        // 播放系统提示音
        myHelper::PlaySound("system");
        // 询问
        CMessageBox::Question(this, tr("[%1] 请求加你为好友，是否接受？\n %2").arg(name).arg(strMsg));
    }
}

/**
 * @brief MainWindow::ParseAddGroupRequest
 * 有人加入了你的群组
 * @param dataVal
 */
void MainWindow::ParseAddGroupRequest(const QJsonValue &dataVal)
{
    if (!dataVal.isObject()) return;
}

/**
 * @brief MainWindow::ParseMyFriendsReply
 * @param dataVal
 */
void MainWindow::ParseGetFriendsReply(const QJsonValue &dataVal)
{
    // data 的 value 是数组
    if (dataVal.isArray()) {
        QJsonArray array = dataVal.toArray();
        int nSize = array.size();
        for (int i = 0; i < nSize; ++i) {
            QJsonObject jsonObj = array.at(i).toObject();
            int nStatus = jsonObj.value("status").toInt();

            QString strHead = jsonObj.value("head").toString();

            // 头像判断，如果不在就申请
            if (!QFile::exists(MyApp::m_strHeadPath + strHead))
            {
                QJsonObject jsonObj;
                jsonObj.insert("from", MyApp::m_nId);
                jsonObj.insert("id",  -2);
                jsonObj.insert("msg", strHead);
                m_tcpSocket->SltSendMessage(GetFile, jsonObj);

                myHelper::Sleep(100);
            }

            QQCell *cell = new QQCell;
            cell->groupName = QString(tr("我的好友"));
            cell->iconPath  = MyApp::m_strHeadPath + jsonObj.value("head").toString();
            cell->type      = QQCellType_Child;
            cell->name      = jsonObj.value("name").toString();
            cell->subTitle  = QString("当前用户状态：%1 ").arg(OnLine == nStatus ? tr("在线") : tr("离线"));
            cell->id        = jsonObj.value("id").toInt();
            cell->status    = nStatus;

            ui->frindListWidget->insertQQCell(cell);
        }
    }

    // 上报我的上线消息
    m_tcpSocket->SltSendOnline();
}

/**
 * @brief MainWindow::ParseGroupFriendsReply
 * @param dataVal
 */
void MainWindow::ParseGetGroupFriendsReply(const QJsonValue &dataVal)
{
    // data 的 value 是数组
    if (dataVal.isArray()) {
        QJsonArray array = dataVal.toArray();
        int nGroupId = array.at(0).toInt();

        // 将数据更新至界面
        // 判断与该用户是否有聊天窗口，如果有弹出窗口
        foreach (ChatWindow *window, m_chatGroupWindows) {
            if (window->GetUserId() == nGroupId) {
                window->UpdateUserStatus(dataVal);
                window->show();
                return;
            }
        }
    }
}

/**
 * @brief MainWindow::ParseRefreshFriendsReply
 * 刷新好友
 * @param dataVal
 */
void MainWindow::ParseRefreshFriendsReply(const QJsonValue &dataVal)
{
    // data 的 value 是数组
    if (dataVal.isArray()) {
        QJsonArray array = dataVal.toArray();
        int nSize = array.size();
        for (int i = 0; i < nSize; ++i) {
            QJsonObject jsonObj = array.at(i).toObject();
            int nId = jsonObj.value("id").toInt();
            int nStatus = jsonObj.value("status").toInt();
            QString strHead = jsonObj.value("head").toString();

            QList<QQCell *> friends = ui->frindListWidget->getCells();
            foreach (QQCell *cell, friends.at(0)->childs) {
                if (cell->id == nId) {
                    cell->SetSubtitle(strHead);
                    cell->SetSubtitle(QString("当前用户状态：%1 ").arg(OnLine == nStatus ? tr("在线") : tr("离线")));
                }
            }

            ui->frindListWidget->upload();
        }
    }
}

/**
 * @brief MainWindow::ParseRefreshGroupFriendsReply
 * 刷新组
 * @param dataVal
 */
void MainWindow::ParseRefreshGroupFriendsReply(const QJsonValue &dataVal)
{
    // data 的 value 是数组
    if (dataVal.isArray()) {
        QJsonArray array = dataVal.toArray();
        int nSize = array.size();
        for (int i = 0; i < nSize; ++i) {
            //            QJsonObject jsonObj = array.at(i).toObject();
            //            int nId = jsonObj.value("id").toInt();
            //            int nStatus = jsonObj.value("status").toInt();

            //            QList<QQCell *> friends = ui->groupListWidget->getCells();
            //            foreach (QQCell *cell, friends.at(0)->childs) {
            //                if (cell->id == nId) {
            //                    cell->SetSubtitle(QString("当前用户状态：%1 ").arg(OnLine == nStatus ? tr("在线") : tr("离线")));
            //                }
            //            }

            //            ui->groupListWidget->upload();
        }
    }
}

/**
 * @brief MainWindow::ParseMessageReply
 * @param dataVal
 */
void MainWindow::ParseFriendMessageReply(const QJsonValue &dataVal)
{
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();
        int nId = dataObj.value("id").toInt();
        // 播放系统提示音
        //TestMedia::Instance()->playWav(MyApp::m_strSoundPath + "msg.wav");

        SltReadMessages(dataVal, nId);
    }
}

/**
 * @brief MainWindow::ParseGroupMessageReply
 * 处理群组ID
 * @param dataVal
 */
void MainWindow::ParseGroupMessageReply(const QJsonValue &dataVal)
{
    // 消息格式为object对象
    if (dataVal.isObject()) {
        QJsonObject dataObj = dataVal.toObject();
        int nId = dataObj.value("group").toInt();
        // 播放系统提示音
        myHelper::PlaySound("msg");

        // 如果收到消息时有聊天窗口存在，直接添加到聊天记录，并弹出窗口
        foreach (ChatWindow *window, m_chatGroupWindows) {
            if (window->GetUserId() == nId) {
                window->AddMessage(dataVal);
                window->show();
                return;
            }
        }

        // 没有检索到聊天窗口，直接弹出新窗口
        QList<QQCell *> groups = ui->groupListWidget->getCells();
        foreach (QQCell *cell, groups.at(0)->childs) {
            // 有列表的才创建
            if (cell->id == nId) {
                ChatWindow *chatWindow = new ChatWindow();
                connect(chatWindow, SIGNAL(signalSendMessage(quint8,QJsonValue)), m_tcpSocket, SLOT(SltSendMessage(quint8,QJsonValue)));
                connect(chatWindow, SIGNAL(signalClose()), this, SLOT(SltGroupChatWindowClose()));

                // 构建 Json 对象
                QJsonObject json;
                json.insert("id", cell->id);
                json.insert("name", cell->name);

                m_tcpSocket->SltSendMessage(GetMyGroups, json);


                chatWindow->SetCell(cell, 1);
                chatWindow->AddMessage(dataVal);
                chatWindow->show();
                // 添加到当前聊天框
                m_chatGroupWindows.append(chatWindow);
                return;
            }
        }
    }
}

/**
 * @brief MainWindow::AddMyGroups
 * @param dataVal
 */
void MainWindow::AddMyGroups(const QJsonValue &dataVal)
{
    // data 的 value 是数组
    if (dataVal.isArray()) {
        QJsonArray array = dataVal.toArray();
        int nSize = array.size();
        for (int i = 0; i < nSize; ++i) {
            QJsonObject jsonObj = array.at(i).toObject();

            QQCell *cell = new QQCell;
            cell->groupName = QString(tr("我的群组"));
            cell->iconPath  = ":/resource/head/1.bmp";
            cell->type      = QQCellType_Child;
            cell->name      = jsonObj.value("name").toString();
            cell->subTitle  = QString("我的群，我做主...");
            cell->id        = jsonObj.value("id").toInt();;
            cell->SetStatus(OnLine);

            ui->groupListWidget->insertQQCell(cell);
        }
    }
}

/**
 * @brief MainWindow::UpdateFriendStatus
 * 更新好友状态
 * @param nStatus
 * @param dataVal
 */
void MainWindow::UpdateFriendStatus(const quint8 &nStatus, const QJsonValue &dataVal)
{
    if (!dataVal.isObject()) return;
    QJsonObject jsonObj = dataVal.toObject();
    int nId = jsonObj.value("id").toInt();

    QList<QQCell *> groups = ui->frindListWidget->getCells();
    foreach (QQCell *cell, groups.at(0)->childs) {
        if (cell->id == nId) {
            cell->SetStatus(nStatus);
            cell->SetSubtitle(QString("当前用户状态：%1 ").arg(OnLine == nStatus ? tr("在线") : tr("离线")));
        }
    }

    // 更新显示
    ui->frindListWidget->upload();
    //TestMedia::Instance()->playWav(MyApp::m_strSoundPath + "userlogon.wav");
}

/**
 * @brief MainWindow::GetHeadPixmap
 * @param name
 * @return
 */
QString MainWindow::GetHeadPixmap(const QString &name) const
{
    if (QFile::exists(MyApp::m_strHeadPath + name)) {
        return MyApp::m_strHeadPath + name;
    }

    return ":/resource/head/1.bmp";
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

// 托盘菜单
void MainWindow::SltTrayIconMenuClicked(QAction *action)
{
    if ("退出" == action->text()) {
        m_bQuit = true;
        m_tcpSocket->SltSendOffline();
        this->hide();
        QTimer::singleShot(500, this, SLOT(SltQuitApp()));
    }
    else if ("显示主面板" == action->text()) {
        this->show();
    }
    else if (!QString::compare("我在线上", action->text()))
    {
        m_tcpSocket->CheckConnected();
    }
    else if (!QString::compare("离线", action->text()))
    {
        m_tcpSocket->ColseConnected();
    }
}

/**
 * @brief MainWindow::SltSysmenuCliecked
 * 系统菜单点击
 * @param action
 */
void MainWindow::SltSysmenuCliecked(QAction *action)
{
    /*菜单按钮响应*/
    if ("系统设置" == action->text()) {
        SystemSetting *widgetSetting = new SystemSetting();
        widgetSetting->exec();
    }
    else if ("消息管理器" == action->text()) {
        CBaseDialog *dialog = new CBaseDialog(this);
        dialog->show();
    }
    else if ("文件助手" == action->text()) {
        CMessageBox::Infomation(this, "查看文件助手");
    }
    else if ("修改密码" == action->text())
    {
        QString strText = CInputDialog::GetInputText(this, "张三");
        qDebug() << "Input" << strText;
    }
    else if ("帮助" == action->text())
    {

    }
    else if (!QString::compare("连接服务器", action->text()))
    {
        m_tcpSocket->CheckConnected();
    }
    else if ("升级" == action->text()) {
        if (QDialog::Accepted == CMessageBox::Question(this, "检测到新版本，是否升级？"))
        {

            CMessageBox::Infomation(this, "系统升级成功");
        }
    }
}

/**
 * @brief MainWindow::SltQuitApp
 * 程序退出处理
 */
void MainWindow::SltQuitApp()
{
    // 关闭所有的聊天窗口
    foreach (ChatWindow *window, m_chatFriendWindows) {
        window->close();
    }

    foreach (ChatWindow *window, m_chatGroupWindows) {
        window->close();
    }

    // 关闭socket
    delete m_tcpSocket;
    m_tcpSocket = NULL;

    delete ui;
    qApp->quit();
}


/**
 * @brief MainWindow::SltHeadPicCutOk
 * 头像裁剪完成
 */
void MainWindow::SltHeadPicCutOk()
{
    // 构建 Json 对象
    QJsonObject json;
    QFileInfo fileInfo(MyApp::m_strHeadFile);

    json.insert("id",  MyApp::m_nId);
    json.insert("head", fileInfo.fileName());
    json.insert("friends", DataBaseMagr::Instance()->GetMyFriend(MyApp::m_nId));

    qDebug() << "upload head" << json;

    m_tcpSocket->SltSendMessage(UpdateHeadPic, json);
}

/**
 * @brief MainWindow::SltUpdateUserHead
 * 更新用户头像
 * @param userId
 * @param strHead
 */
void MainWindow::SltUpdateUserHead(const int &userId, const QString &strHead)
{
    QList<QQCell *> friends = ui->frindListWidget->getCells();
    foreach (QQCell *cell, friends.at(0)->childs) {
        if (cell->id == userId) {
            cell->SetIconPath(strHead);
        }
    }

    ui->frindListWidget->upload();
}

/**
 * @brief MainWindow::DownloadFriendHead
 * 到服务器下载用户头像
 * @param strHead
 */
void MainWindow::DownloadFriendHead(const int &userId, const QString &strHead)
{
    if (QFile::exists(strHead)) return;

    // 连接服务器，等服务器将文件下发过来
    ui->widgetHead->DownloadFriendHead(userId);

    // 延迟一点发送
    myHelper::Sleep(100);

    QJsonObject jsonReply;
    jsonReply.insert("from", MyApp::m_nId);
    jsonReply.insert("id",  -2);
    jsonReply.insert("msg", strHead);

    m_tcpSocket->SltSendMessage(GetFile, jsonReply);
    qDebug() << "get file" << jsonReply;
}

/**
 * @brief MainWindow::ParseUpFriendHead
 * 更新好友头像
 * @param dataVal
 */
void MainWindow::ParseUpFriendHead(const QJsonValue &dataVal)
{
    if (!dataVal.isObject()) return;
    QJsonObject jsonObj = dataVal.toObject();
    int nId = jsonObj.value("id").toInt();
    QString strHead = jsonObj.value("head").toString();

    DownloadFriendHead(nId, strHead);
}
