#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QLockFile>
#include <QMessageBox>

#include <QSharedMemory>
#include <QSystemSemaphore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /***************************************************************************
     *      @brief QSharedMemory
     ***************************************************************************/

//    // 临界区
//    QSystemSemaphore semaphore("semaphore",1,QSystemSemaphore::Open);
//    semaphore.acquire();

//    // QSharedMemory
//    QSharedMemory memory("memory");
//    if (!memory.create(1))
//    {
//        QString dlgTitle="information";
//        QString str="实例已运行,该APP只允许运行一个实例";
//        QMessageBox::information(nullptr, dlgTitle,str,QMessageBox::Yes);
//        semaphore.release();
//        return 0;
//    }
//    semaphore.release();

    /***************************************************************************
     *      @brief QSharedMemory
     ***************************************************************************/

    MainWindow w;
    w.show();
    return a.exec();
}
