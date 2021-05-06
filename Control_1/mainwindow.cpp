#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "skin.h"

#include <QtDebug>
#include <QTimer>

#include <QPushButton>
#include <QMessageBox>
#include <QMovie>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initInterface();
    inttControl();
}
void MainWindow::initInterface()
{
    ui->btn_2->setCheckable(true);
    ui->btn_2->autoExclusive();

//    ui->btn_3->setStyleSheet("QPushButton{border: 0px solid;background-color:#F65345;color:#FFFFFF;border-radius: 15px;}\
//                                QPushButton:disabled{background-color:#DCDCDC;border-radius:15px;color:#FFFFFF;}\
//                                QPushButton:hover{border: 0px solid;background-color:#f67469;border-radius:15px;color:#FFFFFF;opacity:0.2;}\
//                                QPushButton:pressed{background-color:#c44237;border-radius:15px;color:#FFFFFF;}");
//    ui->btn_3->setFixedSize(ui->btn_1->width(),ui->btn_1->height());

//    ui->btn_4->setStyleSheet("QPushButton{border-image: url(:/images/calendar/ic_btn.png)} \
//                                QPushButton:pressed{border-image: url(:/images/calendar/ic_btn_pre.png)}");
//    ui->btn_4->setFixedSize(ui->btn_1->width(),ui->btn_1->height());


//    ui->tbtn_2->setStyleSheet("QToolButton{border: 0px solid;background-color:#F65345;color:#FFFFFF;}\
//                                QToolButton:disabled{background-color:#DCDCDC;color:#FFFFFF;}\
//                                QToolButton:hover{border: 0px solid;background-color:#f67469;color:#FFFFFF;opacity:0.2;}\
//                                QToolButton:checked{background-color:#c44237;color:#FFFFFF;}");
//    ui->tbtn_3->setFocusPolicy(Qt::NoFocus);
//    ui->tbtn_3->setAutoRaise(true);
//    ui->tbtn_3->setIcon(QIcon(":/images/camera/take_pressed.png"));
//    ui->tbtn_3->setIconSize(QSize(45,45));

//    ui->tbtn_4->setStyleSheet("QToolButton{border-image: url(:/images/camera/take.png)} \
//                                QToolButton:pressed{border-image: url(:/images/camera/take_pressed.png)}");

    ui->tbtn_2->setCheckable(true);


    QStringList sizelist;
    sizelist<< "12"<<"14"<<"16"<<"18"<<"20";
    ui->comboBox->addItems(sizelist);

//    QFontDatabase database;
//    foreach (const QString &family, database.families())
//    {
//         qDebug()<<family;
//    }

//    ui->spinBox_G->setStyleSheet("QSpinBox{border:1px solid #242424;border-radius:3px;padding:2px;background:none;selection-background-color:#484848;selection-color:#DCDCDC;}\
//                                 QSpinBox:hover{border:1px solid #242424;}\
//                                 QSpinBox:focus{border-width:0px;border-radius:0px;} \
//                                 QSpinBox::up-button{image:url(:/qss/psblack/add_top.png);width:10px;height:10px;padding:2px 5px 0px 0px;} \
//                                 QSpinBox::up-button:pressed{top:-2px;} \
//                                 QSpinBox::down-button{image:url(:/qss/psblack/add_bottom.png);width:10px;height:10px;padding:0px 5px 2px 0px;} \
//                                 QSpinBox::down-button:pressed{top:-2px;} \
//                                 ");



    ui->spinBox_R->setRange(0,255);
    ui->spinBox_G->setRange(0,255);
    ui->doubleSpinBox_B->setRange(0,255);

    ui->spinBox_R->setValue(0);
    ui->spinBox_G->setValue(0);
    ui->doubleSpinBox_B->setValue(0);

    ui->lab_1->setText("123");
    QPixmap pix(":/images/camera/take_pressed.png");
    ui->lab_2->setPixmap(pix);
    QMovie *movie = new QMovie(":/images/etc/2021-3-20.gif");
    movie->setScaledSize(ui->lab_2->size());
    movie->start();
    ui->lab_3->setMovie(movie);
    ui->lab_3->setScaledContents(true);
    ui->lab_4->setText("<p style=\"color:red;font-size:16px;\"> hello <b style=\"color:black;font-size:22px;\">QLabel</b></p>");

    ui->lineEdit_1->setText("123");
    ui->lineEdit_2->setPlaceholderText("默认显示");
    QRegExp regx("[0-9]+12");
    ui->lineEdit_3->setValidator(new QRegExpValidator(regx, this));
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);

}
void MainWindow::inttControl()
{
    connect(ui->action_bar,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));
    connect(ui->action_btn,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));
    connect(ui->action_datetime,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));
    connect(ui->action_etc,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));
    connect(ui->action_text,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));
    connect(ui->action_about,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));
    connect(ui->action_exit,SIGNAL(triggered()),this,SLOT(on_setCurrentIndex()));

    QList<QPushButton*> btnlist = ui->stackedWidget->findChildren<QPushButton*>();
    foreach(auto btn, btnlist)
    {
        connect(btn,SIGNAL(clicked()),this,SLOT(pushbutton_clicked()));
    }

    QList<QToolButton*> tbtnlist = ui->stackedWidget->findChildren<QToolButton*>();
    foreach(auto tbtn, tbtnlist)
    {
        connect(tbtn,SIGNAL(clicked()),this,SLOT(toolbutton_clicked()));
    }

    QList<QRadioButton*> rbtnlist = ui->stackedWidget->findChildren<QRadioButton*>();
    foreach(auto rbtn, rbtnlist)
    {
        connect(rbtn,SIGNAL(clicked()),this,SLOT(radiobutton_clicked()));
    }

    QList<QCheckBox*> cboxlist = ui->stackedWidget->findChildren<QCheckBox*>();
    foreach(auto cbox, cboxlist)
    {
        connect(cbox,SIGNAL(stateChanged(const int)),this,SLOT(checkbox_stateChange(const int)));
    }

    QList<QLineEdit*> linelist = ui->stackedWidget->findChildren<QLineEdit*>();
    foreach(auto line, linelist)
    {
        connect(line,SIGNAL(textEdited(const QString)),this,SLOT(lineedit_textEdited(const QString)));
        connect(line,SIGNAL(returnPressed()),this,SLOT(lineedit_returnPressed()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_setCurrentIndex()
{
    QAction* action= qobject_cast<QAction*>(sender());
    //QString actionName = QObject::sender()->objectName();
    //qDebug()<<actionName;

    if(action->objectName()=="action_datetime")
        ui->stackedWidget->setCurrentIndex(0);
    else if(action->objectName()=="action_etc")
        ui->stackedWidget->setCurrentIndex(1);
    else if(action->objectName()=="action_btn")
        ui->stackedWidget->setCurrentIndex(2);
    else if(action->objectName()=="action_text")
        ui->stackedWidget->setCurrentIndex(3);
    else if(action->objectName()=="action_bar")
        ui->stackedWidget->setCurrentIndex(4);
    else if(action->objectName()=="action_about")
    {
        QString dlgTitle="about Qt";
        QMessageBox::aboutQt(this, dlgTitle);
    }
    else if(action->objectName()=="action_exit")
        exit(0);

    ui->statusBar->showMessage(QString("切换到:%1页").arg(action->text()));
}
void MainWindow::pushbutton_clicked()
{   
    QPushButton* btn= qobject_cast<QPushButton*>(sender());
    if(btn->objectName()=="btn_2")
    {
        if(btn->isChecked())
            ui->statusBar->showMessage(QString("%1按下").arg(btn->objectName()));
        else
            ui->statusBar->showMessage(QString("%1弹起").arg(btn->objectName()));

        return;
    }

    ui->statusBar->showMessage(QString("%1被点击").arg(btn->objectName()));
}
void MainWindow::toolbutton_clicked()
{
    QToolButton* tbtn= qobject_cast<QToolButton*>(sender());

    if(tbtn->objectName()=="tbtn_2")
    {
        if(tbtn->isChecked())
            ui->statusBar->showMessage(QString("%1按下").arg(tbtn->objectName()));
        else
            ui->statusBar->showMessage(QString("%1弹起").arg(tbtn->objectName()));

        return;

    }

    ui->statusBar->showMessage(QString("%1被点击").arg(tbtn->objectName()));
}

void MainWindow::radiobutton_clicked()
{
    QRadioButton* rbtn= qobject_cast<QRadioButton*>(sender());

    ui->statusBar->showMessage(QString("%1被选中").arg(rbtn->objectName()));
}

void MainWindow::checkbox_stateChange(const int state)
{
    QCheckBox* cbox= qobject_cast<QCheckBox*>(sender());

    if(state)
        ui->statusBar->showMessage(QString("%1被选中").arg(cbox->objectName()));
    else
        ui->statusBar->showMessage(QString("%1未选中").arg(cbox->objectName()));

}
void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    qApp->setFont(f);
    ui->statusBar->showMessage(QString("当前字体设置为:%1").arg(f.family()));
}


void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QFont font = qApp->font();
    font.setPointSize(arg1.toInt());
    qApp->setFont(font);
    this->update();
    ui->statusBar->showMessage(QString("当前字号:%1").arg(arg1));
}

void MainWindow::on_spinBox_R_valueChanged(int )
{
    ui->gBox_Box->setStyleSheet(QString("color:rgb(%1, %2, %3)").arg(ui->spinBox_R->value()).arg(ui->spinBox_G->value()).arg(ui->doubleSpinBox_B->value()));
    ui->statusBar->showMessage(QString("字体颜色:rgb(%1, %2, %3)").arg(ui->spinBox_R->value()).arg(ui->spinBox_G->value()).arg(ui->doubleSpinBox_B->value()));
}
void MainWindow::on_spinBox_G_valueChanged(int )
{
    ui->gBox_Box->setStyleSheet(QString("color:rgb(%1, %2, %3)").arg(ui->spinBox_R->value()).arg(ui->spinBox_G->value()).arg(ui->doubleSpinBox_B->value()));
    ui->statusBar->showMessage(QString("字体颜色:rgb(%1, %2, %3)").arg(ui->spinBox_R->value()).arg(ui->spinBox_G->value()).arg(ui->doubleSpinBox_B->value()));
}
void MainWindow::on_doubleSpinBox_B_valueChanged(double )
{
    ui->gBox_Box->setStyleSheet(QString("color:rgb(%1, %2, %3)").arg(ui->spinBox_R->value()).arg(ui->spinBox_G->value()).arg(ui->doubleSpinBox_B->value()));
    ui->statusBar->showMessage(QString("字体颜色:rgb(%1, %2, %3)").arg(ui->spinBox_R->value()).arg(ui->spinBox_G->value()).arg(ui->doubleSpinBox_B->value()));
}

void MainWindow::handletimeout()
{
    static int curr=0;
    ui->horizontalScrollBar->setValue(curr);
    ui->verticalScrollBar->setValue(curr);
    if(curr++>ui->horizontalScrollBar->maximum())
        curr=0;
}

void MainWindow::lineedit_returnPressed()
{
    QLineEdit* line= qobject_cast<QLineEdit*>(sender());
    QString str= line->text();
    if(str.isEmpty())
        return;

    line->clear();
    ui->plainTextEdit->appendPlainText(">>> "+ str);
}

void MainWindow::lineedit_textEdited(const QString str)
{
    //QLineEdit* line= qobject_cast<QLineEdit*>(sender());
    ui->textBrowser->clear();
    ui->textBrowser->append(str);
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    ui->dateEdit->setDate(date);
    ui->statusBar->showMessage(QString("当前日期:%1").arg(date.toString("yyyy-MM-dd")));
}

void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    ui->calendarWidget->setSelectedDate(date);
    ui->statusBar->showMessage(QString("当前日期:%1").arg(date.toString("yyyy-MM-dd")));
}

void MainWindow::on_timeEdit_userTimeChanged(const QTime &time)
{
    ui->statusBar->showMessage(QString("当前时间:%1").arg(time.toString("hh:mm:ss")));
}

void MainWindow::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    ui->statusBar->showMessage(QString("当前时间日期:%1").arg(dateTime.toString("yyyy-MM-dd hh:mm:ss")));
}
