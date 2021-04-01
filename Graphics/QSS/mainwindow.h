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

    bool        windowsDrag;
    QPoint      mouseStartPoint;
    QPoint      windowTopLeftPoint;

protected:
    //拖拽窗口
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_tbtn_main_clicked();

    void on_tbtn_kill_clicked();

    void on_tbtn_clean_clicked();

    void on_tbtn_repari_clicked();

    void on_tbtn_accelerate_clicked();

    void on_tbtn_function_clicked();

    void on_tbtn_vault_clicked();

    void on_tbtn_soft_clicked();

    void on_tbtn_game_clicked();

    void on_toolButton_27_clicked();

private:
    Ui::MainWindow *ui;

    bool m_move;
    QPoint m_startPoint;
    QPoint m_windowPoint;
};

#endif // MAINWINDOW_H
