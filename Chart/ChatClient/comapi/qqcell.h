#ifndef QQCELL_H
#define QQCELL_H

#include <QString>
#include <QList>

typedef enum tagQQCellType_e
{
    QQCellType_Group = 0,
    QQCellType_Child = 1,
    QQCellType_GroupEx = 2,
} QQCellType_e;

class QQCell
{
public:
    QQCell();

    void SetSubtitle(const QString &text);
    void SetIconPath(const QString &path);
    void SetStatus(const quint8 &val);
public:
    QString iconPath;
    QString name;
    QString subTitle;

    QString ipaddr;
    QString groupName;      //组名
    int     id;             //QQ号，作为标识
    int     status;

    //
    QQCellType_e type;

    //下面几项都是用于组(QQCellType_Group)状态, 对于子(QQCellType_Child)状态无用
    //是否是打开状态
    bool isOpen;
    //子项
    QList<QQCell *> childs;
};

#endif // QQCELL_H
