/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : iteminfo.h --- ItemInfo
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#ifndef ITEMINFO_H
#define ITEMINFO_H

#include <QtCore/qglobal.h>
#include <QPixmap>
#include <QRectF>
#include "unit.h"

class ItemInfo
{
public:
    ItemInfo();
    ItemInfo(const QString& strName, const QString& datetime, const QString &pixmap, const QString &text, const QString& strSize, const quint8 &orientation = Right, const quint8 &msgType = Text);

    ~ItemInfo();

public:
    // public interfaces

    void SetName(const QString &text);
    QString GetName() const;

    void SetDatetime(const QString &text);
    QString GetDatetime() const;

    void SetHeadPixmap(const QString &pixmap);
    QString GetStrPixmap() const;

    void SetText(const QString& text);
    QString GetText() const;

    void SetFileSizeString(const QString& strSize);
    QString GetFileSizeString() const;

    void SetOrientation(quint8 orientation);
    quint8 GetOrientation() const;

    void SetMsgType(const quint8 &msgType);
    quint8 GetMsgType() const;

    void SetItemHeight(qreal itemHeight);
    qreal GetItemHeight() const;

    void SetBobbleRect(const QRectF &bobbleRect);
    QRectF GetBobbleRect() const;

private:
    // 消息的标题，比如名字和时间
    QString     m_strName;
    // 消息日期
    QString     m_strDatetime;
    // 头像图片
    QString     m_strPixmap;
    // 消息内容
    QString     m_strText;
    // 如果是文件，会显示文件大小
    QString     m_strSize;

    // 气泡方向
    quint8      m_orientation;
    // 消息内容
    quint8      m_msgType;
    // 暂时没有用
    qreal       m_itemHeight;
    // 窗口的坐标，在点击查询时需要访问
    QRectF      m_bobbleRect;
    // 消息状态
    quint8      m_nStatus;
};


#endif // ITEMINFO_H
