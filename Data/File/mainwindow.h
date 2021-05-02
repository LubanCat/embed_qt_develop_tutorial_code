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

private slots:
    void on_btn_QDataStream_write_clicked();
    void on_btn_QDataStream_read_clicked();
    void on_btn_QTextStream_write_clicked();
    void on_btn_QTextStream_read_clicked();
    void on_btn_QFile_write_clicked();
    void on_btn_QFile_read_clicked();
    void on_btn_QFileInfo_file_clicked();
    void on_btn_QFileInfo_dir_clicked();
    void on_btn_clear_clicked();
    void on_cBox_Codec_currentIndexChanged(const QString &arg1);

    void on_btn_Watch_clicked();
    void handDirChanged(const QString &path);
    void handFileChanged(const QString &path);

    void on_btn_creatdir_clicked();

    void on_btn_dirinfo_clicked();

private:
    Ui::MainWindow *ui;

    QString m_datastreampath;
    QString m_textstreampath;
    QString m_filepath;
};

#endif // MAINWINDOW_H
