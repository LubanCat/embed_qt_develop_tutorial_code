/***************************************************************************
 * Copyright (C) 2021 - All Rights Reserved by
 * 文 件 名 : mainwindow.cpp ---
 * 作 者    : taotaodiy(野火电子)
 * 论 坛    : http://www.firebbs.cn
 * 文 档    : https://doc.embedfire.com/linux/qt/embed/zh/latest/index.html
 * 代码仓库 : https://gitee.com/LubanCat/embed_qt_develop_tutorial_code
 * 编写日期 : 2021/04/23
 * 说 明    :
 * 历史纪录 :
 * <作者>        <版本>        <日期>                 <内容>
 * taotaodiy     1.0.0 1      2021/04/23            文件创建
 ****************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFile>
#include <QFont>
#include <QUrl>
#include <QDebug>

#include <QDropEvent>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//初始化变量
void MainWindow::Init()
{
    this->setWindowIcon(QIcon(":/img/icon_logo.png"));
    this->setWindowTitle(tr("记事本"));
    this->setAcceptDrops(true);
    text_Edit_Current = nullptr;
    AddAction();
    CreateMenu();
}
/*
 * @brief   函数功能说明
 * @return  void
 */
void MainWindow::AddAction()
{
    ui->mainToolBar->addAction(ui->action_new);
    ui->mainToolBar->addAction(ui->action_open);
    ui->mainToolBar->addAction(ui->action_save);
    ui->mainToolBar->addAction(ui->action_save_all);
    ui->mainToolBar->addAction(ui->action_saveas);
    ui->mainToolBar->addAction(ui->action_close_current);
    ui->mainToolBar->addAction(ui->action_close_all);
    ui->mainToolBar->addAction(ui->action_undo);
    ui->mainToolBar->addAction(ui->action_redo);
}
void MainWindow::CreateMenu()
{
    QMenu *mainMenu = new QMenu(this);
    mainMenu->setTitle("mainMenu");
    QAction *action1 = new QAction(mainMenu);
    action1->setText("action1");
    //connect(action1,SIGNAL(triggered()),this, SLOT(handaction1_triggered()));
    QAction *action2 = new QAction(mainMenu);
    action2->setText("action2");
    //connect(action2,SIGNAL(triggered()),this, SLOT(handaction2_triggered()));
    QList<QAction *> actionlist;
    actionlist << action1 << action2;

    QMenu *nextMenu = new QMenu(mainMenu);
    nextMenu->setTitle("nextMenu&N");
    QAction *action3 = new QAction(nextMenu);
    action3->setText("action3");
    QAction *action4 = new QAction(nextMenu);
    action4->setText("action4");
    nextMenu->addAction(action3);
    nextMenu->addAction(action4);

    mainMenu->addActions(actionlist);
    mainMenu->addMenu(nextMenu);

    ui->menuBar->addMenu(mainMenu);
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    foreach(QUrl url,urls)
    {
        qDebug()<<url;
        Action_Open(url.toLocalFile());
    }
}
/***************************************************************************
 *      @brief
 ***************************************************************************/
//响应菜单栏新建操作窗口
void MainWindow::on_action_new_triggered()
{
    Action_New();
}
//响应菜单栏打开文件
void MainWindow::on_action_open_triggered()
{
    Action_Open();
}
//保存文件
void MainWindow::on_action_save_triggered()
{
    Action_Save();
}
void MainWindow::on_action_save_all_triggered()
{
    Action_Save_All();
}
//将文件另存为
void MainWindow::on_action_saveas_triggered()
{
    Action_Saveas();
}
void MainWindow::on_action_rich_triggered()
{
    Action_SaveHtml();
}


//响应菜单栏关闭当前操作窗口
void MainWindow::on_action_close_current_triggered()
{
    Action_Close_Current();
}
//响应菜单关闭全部可操作窗口
void MainWindow::on_action_close_all_triggered()
{
    Action_Close_All();
}
//响应菜单栏退出
void MainWindow::on_action_exit_triggered()
{
    Action_Exit();
}
//直接调用文本框的复制粘贴等操作
void MainWindow::on_action_undo_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        text_Edit_Current->undo();
        ui->statusBar->showMessage(tr("已撤销"));
    }
}
void MainWindow::on_action_redo_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        text_Edit_Current->redo();
        ui->statusBar->showMessage(tr("已回滚"));
    }
}
void MainWindow::on_action_cut_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        text_Edit_Current->cut();
        ui->statusBar->showMessage(tr("已剪切"));
    }
}
void MainWindow::on_action_copy_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        text_Edit_Current->copy();
        ui->statusBar->showMessage(tr("已复制"));
    }
}
void MainWindow::on_action_paste_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        text_Edit_Current->paste();
        ui->statusBar->showMessage(tr("已粘贴"));
    }
}
void MainWindow::on_action_delete_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        text_Edit_Current->clear();
        ui->statusBar->showMessage(tr("已清空"));
    }
}
void MainWindow::on_action_select_all_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        text_Edit_Current->selectAll();
        ui->statusBar->showMessage(tr("已全选"));
    }
}
//设置字体，颜色，大小等
void MainWindow::on_action_size_plus_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        QFont font=text_Edit_Current->font();
        font.setPointSize(font.pointSize()+5);
        text_Edit_Current->setFont(font);
        ui->statusBar->showMessage(tr("字号增加5"));
    }
}
void MainWindow::on_action_size_redu_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        QFont font=text_Edit_Current->font();
        font.setPointSize(font.pointSize()-5);
        text_Edit_Current->setFont(font);
        ui->statusBar->showMessage(tr("字号减小5"));
    }
}
void MainWindow::on_action_color_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        QColor color = QColorDialog::getColor(Qt::blue);
        text_Edit_Current->setTextColor(color);

        ui->statusBar->showMessage(tr("颜色设置成功"));
    }
}
//设置四种字体，二级菜单
void MainWindow::on_action_1_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        QFont font = text_Edit_Current->font();
        font.setFamily("隶书");
        text_Edit_Current->setFont(font);
    }
}
void MainWindow::on_action_2_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        QFont font = text_Edit_Current->font();
        font.setFamily("宋体");
        text_Edit_Current->setFont(font);
    }
}
void MainWindow::on_action_3_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        QFont font = text_Edit_Current->font();
        font.setFamily("楷体");
        text_Edit_Current->setFont(font);
    }
}
void MainWindow::on_action_4_triggered()
{
    text_Edit_Current=Find_Current_Text_Edit();
    if(text_Edit_Current!=nullptr)
    {
        QFont font = text_Edit_Current->font();
        font.setFamily("黑体");
        text_Edit_Current->setFont(font);
    }
}
//其他
void MainWindow::on_action_brief_triggered()
{
    QString dlgTitle="关于Qt";
    QMessageBox::aboutQt(this, dlgTitle);
}
void MainWindow::on_action_explain_triggered()
{
    QString dlgTitle="关于本教程";
    QString str = "教程地址: https://doc.embedfire.com/linux/qt/embed/zh/latest/index.html\r\n";
    str += "代码仓库: https://gitee.com/LubanCat/embed_qt_develop_tutorial_code\r\n";
    QMessageBox::about(this, dlgTitle, str);
}
/*************************************************
 *
 *                  功能函数
 *
 * ***********************************************/

/*********************查找文本编辑框*********************/
QTextEdit* MainWindow::Find_Current_Text_Edit()
{
    if(ui->tabWidget_list->count()==0)
        return nullptr;

    QWidget *text_Edit = ui->tabWidget_list->currentWidget();
    QString object = text_Edit->objectName();
    foreach(QTextEdit *Edit, editList)
    {
        if(Edit->objectName()==object)
        {
            return Edit;
        }
    }
    return nullptr;
}
/*********************新建对话窗口*********************/
int getNumListFromStr(const QString& str)
{
    int order=0;
    int pos = 0;
    QRegExp num("(\\d+)");  // 匹配数字
    while ((pos = num.indexIn(str, pos)) != -1) {
        order= num.cap(0).toInt();
        pos += num.matchedLength();
    }
    return order;
}
void MainWindow::Action_New()
{
    int index=0;

    //判断已存在的index
    QList<int> indexlist;
    for(int i=0; i<ui->tabWidget_list->count(); i++)
    {
        QString object= ui->tabWidget_list->tabText(i);
        if(object.contains("新建文本_"))
            indexlist<<getNumListFromStr(object);
    }
    for(int i=0; i<=indexlist.count(); i++)
    {
        if(!indexlist.contains(i))
        {
            index=i;
            break;
        }
    }


    QString editName=tr("新建文本_")+QString::number(index);

    QTextEdit *text_Edit = new QTextEdit();
    text_Edit->setObjectName(editName);
    editList.append(text_Edit);

    ui->tabWidget_list->addTab(text_Edit,text_Edit->objectName());
    ui->tabWidget_list->setCurrentWidget(text_Edit);
    ui->tabWidget_list->setTabsClosable(ui->tabWidget_list->currentIndex());
    ui->statusBar->showMessage(tr("新建文件成功"));
}
/***************打开文件 允许打开多个文件 不能重复打开同一个文件****************/
void MainWindow::Action_Open()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("打开文件"),"", tr("文本文件(*.txt)"));

    if(fileName.isNull())
        return ;

    //判断文件是否打开过
    foreach(QTextEdit *Edit, editList)
    {
        if(Edit->objectName() == fileName)
        {
            for(int i=0; i<ui->tabWidget_list->count(); i++)
            {
                QWidget *text_Edit = ui->tabWidget_list->widget(i);
                if(text_Edit->objectName() == fileName)
                {
                    ui->statusBar->showMessage(tr("文件已打开"));
                    ui->tabWidget_list->setCurrentIndex(i);
                }
            }
            return;
        }
    }

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::information(this, tr("Error"),tr("读取错误:&%1").arg(file.errorString()));
        return;
    }

    QTextEdit *text_Edit = new QTextEdit();
    text_Edit->setObjectName(fileName);
    editList.append(text_Edit);

    QStringList pathList = fileName.split("/");
    QStringList filelist = pathList.last().split(".");
    ui->tabWidget_list->addTab(text_Edit,filelist.first());
    ui->tabWidget_list->setCurrentWidget(text_Edit);
    ui->tabWidget_list->setTabsClosable(ui->tabWidget_list->currentIndex());

    QTextStream in(&file);
    text_Edit->setPlainText(in.readAll());
    ui->statusBar->showMessage(tr("打开文件成功"));
}
/***************打开文件 允许打开多个文件 不能重复打开同一个文件****************/
void MainWindow::Action_Open(QString fileName)
{
    if(fileName.isNull())
        return ;

    //判断文件是否打开过
    foreach(QTextEdit *Edit, editList)
    {
        if(Edit->objectName() == fileName)
        {
            for(int i=0; i<ui->tabWidget_list->count(); i++)
            {
                QWidget *text_Edit = ui->tabWidget_list->widget(i);
                if(text_Edit->objectName() == fileName)
                {
                    ui->statusBar->showMessage(tr("文件已打开"));
                    ui->tabWidget_list->setCurrentIndex(i);
                }
            }
            return;
        }
    }

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::information(this, tr("Error"),tr("读取错误:&%1").arg(file.errorString()));
        return;
    }

    QTextEdit *text_Edit = new QTextEdit();
    text_Edit->setObjectName(fileName);
    editList.append(text_Edit);

    QStringList pathList = fileName.split("/");
    QStringList filelist = pathList.last().split(".");
    ui->tabWidget_list->addTab(text_Edit,filelist.first());
    ui->tabWidget_list->setCurrentWidget(text_Edit);
    ui->tabWidget_list->setTabsClosable(ui->tabWidget_list->currentIndex());

    QTextStream in(&file);
    text_Edit->setPlainText(in.readAll());
    ui->statusBar->showMessage(tr("打开文件成功"));
}
/***************保存文件 ****************/
void MainWindow::Action_Save()
{
    if(editList.isEmpty())
    {
        ui->statusBar->showMessage("未打开文件");
        return;
    }

    QWidget *text_Edit = ui->tabWidget_list->currentWidget();
    QString object = text_Edit->objectName();
    foreach(QTextEdit *Edit, editList)
    {
        if(Edit->objectName()== object)
        {
            QString file_path;
            if(object.left(5)==tr("新建文本_"))
                file_path=tr("./")+object+tr(".txt");
            else
                file_path=object;

            QFile file(file_path);//文件命名

            if (!file.open(QFile::WriteOnly | QFile::Text))		//检测文件是否打开
            {
                QMessageBox::information(this, "错误", "文本路径错误");
                return ;
            }
            QTextStream out(&file);					//分行写入文件
            out << Edit->toPlainText();
            ui->statusBar->showMessage(tr("文件保存成功,保存路径：")+file_path);

            break;
        }
    }
}
/***************保存所有打开文件****************/
void MainWindow::Action_Save_All()
{
    for(int i=0; i<editList.count();i++)
    {
        QTextEdit *Edit = editList.at(i);

        if(Edit->toPlainText().isEmpty())
            continue;

        QString object=Edit->objectName();
        QString file_path;
        if(object.left(5)==tr("新建文本_"))
            file_path=tr("./")+object+tr(".txt");
        else
            file_path=object;

        QFile file(file_path);//文件命名
        if (!file.open(QFile::WriteOnly | QFile::Text))		//检测文件是否打开
        {
            QMessageBox::information(this, tr("错误"), tr("文件路径错误"));
            return ;
        }
        QTextStream out(&file);					//分行写入文件
        out << Edit->toPlainText();

        ui->statusBar->showMessage(tr("文件保存成功,保存路径：")+file_path);
    }
}
/***************将文件另存为****************/
void MainWindow::Action_Saveas()
{
    if(editList.count()<1)
    {
        ui->statusBar->showMessage("未打开文件");
        return;
    }

    QWidget *text_Edit = ui->tabWidget_list->currentWidget();
    QString object = text_Edit->objectName();
    foreach(QTextEdit *Edit, editList)
    {
        if(Edit->objectName()== object)
        {
            QString file_path = QFileDialog::getSaveFileName(this,tr("文件另存为"), object,tr("文本文件(*.txt)"));
            QFile file(file_path);//文件命名
            if (!file.open(QFile::WriteOnly | QFile::Text))		//检测文件是否打开
            {
                QMessageBox::information(this, "提示", "取消另存");
                return ;
            }
            QTextStream out(&file);					//分行写入文件
            out << Edit->toPlainText();

            ui->statusBar->showMessage(tr("文件保存成功,保存路径：")+file_path);

            QStringList pathList = file_path.split("/");

            Edit->setObjectName(file_path);
            text_Edit->setObjectName(file_path);
            ui->tabWidget_list->setTabText(ui->tabWidget_list->currentIndex(),pathList.last());
            break;
        }
    }
}

void MainWindow::Action_SaveHtml()
{
    if(editList.count()<1)
    {
        ui->statusBar->showMessage("未打开文件");
        return;
    }

    QWidget *text_Edit = ui->tabWidget_list->currentWidget();
    QString object = text_Edit->objectName();
    foreach(QTextEdit *Edit, editList)
    {
        if(Edit->objectName()== object)
        {
            QString file_path = QFileDialog::getSaveFileName(this,tr("文件另存为"), object,tr("文本文件(*.html)"));
            QFile file(file_path);//文件命名
            if (!file.open(QFile::WriteOnly | QFile::Text))		//检测文件是否打开
            {
                QMessageBox::information(this, "提示", "取消另存");
                return ;
            }
            QTextStream out(&file);					//分行写入文件
            out << Edit->toHtml();

            ui->statusBar->showMessage(tr("文件保存成功,保存路径：")+file_path);

            QStringList pathList = file_path.split("/");

            Edit->setObjectName(file_path);
            text_Edit->setObjectName(file_path);
            ui->tabWidget_list->setTabText(ui->tabWidget_list->currentIndex(),pathList.last());
            break;
        }
    }
}
/*******************关闭当前可操作窗口*********************/
void MainWindow::Action_Close_Current()
{
    if(ui->tabWidget_list->count()<1)
    {
        return;
    }

    QWidget *text_Edit = ui->tabWidget_list->currentWidget();
    ui->tabWidget_list->removeTab(ui->tabWidget_list->currentIndex());

    QString object = text_Edit->objectName();


    for(int i=0; i<editList.count(); i++)
    {
        QTextEdit *Edit = editList.at(i);
        if(Edit->objectName()==object)
        {
            delete editList.takeAt(i);
            break;
        }
    }

    ui->tabWidget_list->setCurrentIndex(0);
}
/*******************关闭全部可操作窗口*********************/
void MainWindow::Action_Close_All()
{
    int count=0;
    while (!editList.isEmpty())
    {
        delete editList.takeFirst();
        ui->tabWidget_list->removeTab(count++);
    }
}
/*********************程序退出操作*********************/
void MainWindow::Action_Exit()
{
    exit(0);
}

/*********************关闭选中窗口*********************/
void MainWindow::on_tabWidget_list_tabCloseRequested(int index)
{
    QWidget *text_Edit = ui->tabWidget_list->widget(index);
    ui->tabWidget_list->removeTab(index);

    QString object = text_Edit->objectName();


    for(int i=0; i<editList.count(); i++)
    {
        QTextEdit *Edit = editList.at(i);
        if(Edit->objectName()==object)
        {
            delete editList.takeAt(i);
            break;
        }
    }

    ui->tabWidget_list->setCurrentIndex(0);
}
