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
    QString curPath=QDir::currentPath();//???????????????????????????
    QString dlgTitle="????????????"; //???????????????
    QString filter="????????????(*.txt);;????????????(*.jpg *.gif *.png);;????????????(*.*)"; //???????????????
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
    ui->textEdit->append("?????????:"+fileinfo.baseName()+" "+"????????????:"+fileinfo.completeSuffix()+" "+"????????????:"+QString::number(fileinfo.size()));
    ui->textEdit->append("????????????:"+fileinfo.absoluteFilePath());
    ui->textEdit->append("???????????????:"+fileinfo.owner()+" "+"?????????ID:"+QString::number(fileinfo.ownerId())+" "+"???????????????:"+fileinfo.group()+" "+"?????????ID:"+QString::number(fileinfo.groupId()));
    ui->textEdit->append("????????????:"+QString::number(fileinfo.permissions()));
    ui->textEdit->append("??????????????????:"+fileinfo.birthTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("??????????????????:"+fileinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("??????????????????:"+fileinfo.lastRead().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("??????????????????:"+fileinfo.metadataChangeTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::on_btn_QFileInfo_dir_clicked()
{
    QString curPath=QDir::currentPath();//???????????????????????????
    QString dlgTitle="???????????????"; //???????????????
    QString dirname=QFileDialog::getExistingDirectory(this,dlgTitle,curPath);
    if (dirname.isEmpty())
        return;

    QFileInfo dirinfo(dirname);
    if(!dirinfo.exists())
        return;

    ui->textEdit->clear();
    ui->textEdit->append("????????????:"+dirinfo.baseName());
    ui->textEdit->append("????????????:"+dirinfo.absoluteFilePath());
    ui->textEdit->append("??????????????????:"+dirinfo.owner()+" "+"?????????ID:"+QString::number(dirinfo.ownerId())+" "+"??????????????????:"+dirinfo.group()+" "+"?????????ID:"+QString::number(dirinfo.groupId()));
    ui->textEdit->append("???????????????:"+QString::number(dirinfo.permissions()));
    ui->textEdit->append("?????????????????????:"+dirinfo.birthTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("??????????????????:"+dirinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("??????????????????:"+dirinfo.lastRead().toString("yyyy-MM-dd hh:mm:ss"));
    ui->textEdit->append("??????????????????:"+dirinfo.metadataChangeTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::on_btn_creatdir_clicked()
{
    QString curPath=QDir::currentPath();//???????????????????????????
    QString dlgTitle="???????????????"; //???????????????
    QString dirpath=QFileDialog::getExistingDirectory(this,dlgTitle,curPath);
    if (dirpath.isEmpty())
        return;

    dlgTitle="???????????????";
    QString str="???????????????";
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
    QString curPath=QDir::currentPath();//???????????????????????????
    QString dlgTitle="???????????????"; //???????????????
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
            ui->textEdit->append("?????????:"+info.baseName()+" "+"????????????:"+info.absoluteFilePath());
        }
    }
}

void MainWindow::on_btn_Watch_clicked()
{
    QString curPath=QDir::currentPath();//???????????????????????????
    QString dlgTitle="???????????????"; //???????????????
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
    QString str="????????????:"+path;
    QMessageBox::information(this, dlgTitle, str,QMessageBox::Yes);
}
void MainWindow::handDirChanged(const QString &path)
{
    QString dlgTitle="information";
    QString str="???????????????:"+path;
    QMessageBox::information(this, dlgTitle, str,QMessageBox::Yes);
}
