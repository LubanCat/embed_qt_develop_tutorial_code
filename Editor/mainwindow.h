#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTextEdit>

#define MAX_TEXT_EDIT 12

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void Init();
    void AddAction();
    void CreateMenu();
    void Action_New();
    void Action_Open();
    void Action_Open(QString fileName);
    void Action_Save();
    void Action_Save_All();
    void Action_Saveas();
    void Action_SaveHtml();
    void Action_Close_Current();
    void Action_Close_All();
    void Action_Exit();
    QTextEdit *Find_Current_Text_Edit();

private slots:
    void on_action_exit_triggered();
    void on_action_open_triggered();
    void on_action_close_current_triggered();
    void on_action_close_all_triggered();
    void on_action_new_triggered();
    void on_action_save_triggered();
    void on_action_save_all_triggered();
    void on_action_saveas_triggered();
    void on_action_rich_triggered();

    void on_action_undo_triggered();
    void on_action_redo_triggered();
    void on_action_cut_triggered();
    void on_action_copy_triggered();
    void on_action_paste_triggered();
    void on_action_delete_triggered();
    void on_action_select_all_triggered();
    void on_action_size_plus_triggered();
    void on_action_size_redu_triggered();
    void on_action_color_triggered();
    void on_action_brief_triggered();
    void on_action_explain_triggered();

    void on_action_1_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void on_action_4_triggered();

    void on_tabWidget_list_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;

    QList<QTextEdit *> editList;

    QTextEdit *text_Edit_Current;
};

#endif // MAINWINDOW_H
