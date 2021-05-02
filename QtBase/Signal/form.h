#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

    void setLabText(const QString str);
    void setSendStr(QString str_1,QString str_2);

signals:
    void close();
    void sendMainText(const QString str);
    void sendChildText(const QString str);

private slots:
    void showMyself();
    void hideMyself();
    void reiveLabText(const QString str);
    void reiveMainText(const QString str);
    void reiveChildText(const QString str);

    void on_btn_send_1_clicked();
    void on_btn_send_2_clicked();
    void on_btn_hide_clicked();

protected:
    void closeEvent(QCloseEvent*event);

private:
    Ui::Form *ui;
    QString sendstr_1;
    QString sendstr_2;
};

#endif // FORM_H
