#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fileEncode(QString filepath, QString codefilepath);
    void fileDecode(QString codefilepath, QString filepath);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
