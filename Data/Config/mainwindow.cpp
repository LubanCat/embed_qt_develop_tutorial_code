#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "key.h"

#include <QTextStream>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fileEncode("conf/tempfile.txt", "conf/tempfile_code.txt");
    fileDecode("conf/tempfile_code.txt", "conf/tempfile_decode.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileEncode(QString filepath, QString codefilepath)
{
    QFile file(filepath);
    QFile codefile(codefilepath);

    if (file.open(QFile::ReadOnly  | QFile::Text) && codefile.open(QFile::WriteOnly  | QFile::Text))
    {
        QTextStream in(&file),out(&codefile);
        QString line,temp;
        while (in.readLineInto(&line)){
            temp = EncryptionKEY::instance()->EncodeBase64(line);
            out << temp << "\r\n";
        }
        file.close();
        codefile.close();
    }
}

void MainWindow::fileDecode(QString codefilepath, QString filepath)
{
    QFile codefile(codefilepath);
    QFile file(filepath);

    if (codefile.open(QFile::ReadOnly  | QFile::Text) && file.open(QFile::WriteOnly  | QFile::Text))
    {
        QTextStream in(&codefile),out(&file);
        QString line,temp;
        while (in.readLineInto(&line)){
            temp = DecryptKEY::instance()->DecodeBase64(line);
            out << temp << "\r\n";
        }
        codefile.close();
        file.close();
    }
}
