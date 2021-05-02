#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_datastreampath = qApp->applicationDirPath()+"/datastream.dat";
    m_textstreampath = qApp->applicationDirPath()+"/textstream.txt";
    m_filepath = qApp->applicationDirPath()+"/document";

    QList<QByteArray> list = QTextCodec::availableCodecs();

    foreach(QByteArray codec,list)
    {
        ui->cBox_Codec->addItem(codec.data());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_QDataStream_write_clicked()
{
    static int writeCount=0;

    QFile file(m_datastreampath);
    ui->lineEdit->setText(m_datastreampath);
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out << ui->textEdit->toPlainText();
        out << ++writeCount;
        file.close();
    }
}

void MainWindow::on_btn_QDataStream_read_clicked()
{
    QFile file(m_datastreampath);
    ui->lineEdit->setText(m_datastreampath);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        QString str;
        int writeCount;
        in >> str >> writeCount;
        file.close();

        ui->textEdit->clear();
        ui->textEdit->append(str);
        ui->textEdit->append(QString::number(writeCount));
    }
}

void MainWindow::on_btn_QTextStream_write_clicked()
{
    static int writeCount=0;
    QFile file(m_textstreampath);
    ui->lineEdit->setText(m_textstreampath);
    if (file.open(QFile::WriteOnly  | QFile::Truncate))
    {
        QTextStream out(&file);
        out << ui->textEdit->toPlainText() << "\r\n";
        out << "writeCount:" << ++writeCount << "\r\n";
        file.close();
    }
}

void MainWindow::on_btn_QTextStream_read_clicked()
{
    QFile file(m_textstreampath);
    ui->lineEdit->setText(m_textstreampath);
    if (file.open(QFile::ReadOnly  | QFile::Truncate))
    {
        QTextStream in(&file);
        QString line;

        ui->textEdit->clear();
        while (in.readLineInto(&line)){
            ui->textEdit->append(line);
        }
        file.close();
    }
}

void MainWindow::on_cBox_Codec_currentIndexChanged(const QString &arg1)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(arg1.toLocal8Bit()));
}
void MainWindow::on_btn_clear_clicked()
{
    ui->textEdit->clear();
    ui->lineEdit->clear();
}
void MainWindow::on_btn_QFile_write_clicked()
{
    QFile file(m_filepath);
    ui->lineEdit->setText(m_filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    file.write(ui->textEdit->toPlainText().toLocal8Bit());
    file.close();
}

void MainWindow::on_btn_QFile_read_clicked()
{
    QFile file(m_filepath);
    ui->lineEdit->setText(m_filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString str = file.readAll();
    file.close();

    ui->textEdit->clear();
    ui->textEdit->append(str);
}

void MainWindow::on_btn_QFileInfo_file_clicked()
{
    QString curPath=QDir::currentPath();//获取应用程序的路径
    QString dlgTitle="选择文件"; //对话框标题
    QString filter="文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)"; //文件过滤器
    QString filename=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if (filename.isEmpty())
        return;

    QFileInfo fileinfo(filename);
    if(!fileinfo.exists())
        return;

    if(fileinfo.isSymLink())
    {
        fileinfo.setFile(fileinfo.symLinkTarget());
    }

    ui->textEdit->clear();
    ui->textEdit->append("文件名:"+fileinfo.baseName()+" "+"文件格式:"+fileinfo.completeSuffix()+" "+"文件大小:"+QString::number(fileinfo.size()));
    ui->textEdit->append("绝对路径:"+fileinfo.absoluteFilePath());
    ui->textEdit->append("文件所有者:"+fileinfo.owner()+" "+"所有者ID:"+QString::number(fileinfo.ownerId())+" "+"文件所属组:"+fileinfo.group()+" "+"所属组ID:"+QString::number(fileinfo.groupId()));
    ui->textEdit->append("文件权限:"+QString::number(fileinfo.permissions()));
    ui->textEdit->append("文件创建时间:"+fileinfo.birthTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("上次修改时间:"+fileinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("上次读取时间:"+fileinfo.lastRead().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("元数据更时间:"+fileinfo.metadataChangeTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::on_btn_QFileInfo_dir_clicked()
{
    QString curPath=QDir::currentPath();//获取应用程序的路径
    QString dlgTitle="选择文件夹"; //对话框标题
    QString dirname=QFileDialog::getExistingDirectory(this,dlgTitle,curPath);
    if (dirname.isEmpty())
        return;

    QFileInfo dirinfo(dirname);
    if(!dirinfo.exists())
        return;

    ui->textEdit->clear();
    ui->textEdit->append("文件夹名:"+dirinfo.baseName());
    ui->textEdit->append("绝对路径:"+dirinfo.absoluteFilePath());
    ui->textEdit->append("文件夹所有者:"+dirinfo.owner()+" "+"所有者ID:"+QString::number(dirinfo.ownerId())+" "+"文件夹所属组:"+dirinfo.group()+" "+"所属组ID:"+QString::number(dirinfo.groupId()));
    ui->textEdit->append("文件夹权限:"+QString::number(dirinfo.permissions()));
    ui->textEdit->append("文件夹创建时间:"+dirinfo.birthTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("上次修改时间:"+dirinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("上次读取时间:"+dirinfo.lastRead().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("元数据更时间:"+dirinfo.metadataChangeTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::on_btn_creatdir_clicked()
{
    QString curPath=QDir::currentPath();//获取应用程序的路径
    QString dlgTitle="选择文件夹"; //对话框标题
    QString dirpath=QFileDialog::getExistingDirectory(this,dlgTitle,curPath);
    if (dirpath.isEmpty())
        return;

    dlgTitle="新建文件夹";
    QString str="新建文件夹";
    QString defaultstr="NEWDIR";
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;

    bool choose=false;
    QString dirinput = QInputDialog::getText(this, dlgTitle,str, echoMode,defaultstr, &choose);
    if (choose)
    {
        if(dirinput.isEmpty())
            return;
    }


    QDir *dir = new QDir;
    if(!dir->exists(dirpath+"/"+dirinput))
    {
        dir->mkdir(dirpath+"/"+dirinput);
    }
}

void MainWindow::on_btn_dirinfo_clicked()
{
    QString curPath=QDir::currentPath();//获取应用程序的路径
    QString dlgTitle="选择文件夹"; //对话框标题
    QString dirpath=QFileDialog::getExistingDirectory(this,dlgTitle,curPath);
    if (dirpath.isEmpty())
        return;

    QDir dir(dirpath);
    QStringList files = dir.entryList();

    ui->textEdit->clear();
    foreach(QString path,files)
    {
        QFileInfo info(path);
        if(!info.baseName().isEmpty())
        {
            ui->textEdit->append("文件名:"+info.baseName()+" "+"绝对路径:"+info.absoluteFilePath());
        }
    }
}

void MainWindow::on_btn_Watch_clicked()
{
    QString curPath=QDir::currentPath();//获取应用程序的路径
    QString dlgTitle="选择文件夹"; //对话框标题
    QString dirname=QFileDialog::getExistingDirectory(this,dlgTitle,curPath);
    if (dirname.isEmpty())
        return;

    QFileSystemWatcher *fileSystemWatcher = new QFileSystemWatcher(this);
    fileSystemWatcher->addPath(dirname);
    connect(fileSystemWatcher,SIGNAL(directoryChanged(const QString &)),this, SLOT(handDirChanged(const QString &)));
    connect(fileSystemWatcher,SIGNAL(fileChanged(const QString &)),this, SLOT(handFileChanged(const QString &)));
}
void MainWindow::handFileChanged(const QString &path)
{
    QString dlgTitle="information";
    QString str="文件改变:"+path;
    QMessageBox::information(this, dlgTitle, str,QMessageBox::Yes);
}
void MainWindow::handDirChanged(const QString &path)
{
    QString dlgTitle="information";
    QString str="文件夹改变:"+path;
    QMessageBox::information(this, dlgTitle, str,QMessageBox::Yes);
}
