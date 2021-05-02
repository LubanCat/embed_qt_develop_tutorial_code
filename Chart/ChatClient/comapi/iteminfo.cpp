/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : iteminfo.cpp --- ItemInfo
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/

#include "iteminfo.h"
#include "unit.h"

ItemInfo::ItemInfo() :
    m_strName(""),
    m_strDatetime(""),
    m_strPixmap(""),
    m_strText(""),
    m_strSize(""),
    m_orientation(Left),
    m_msgType(Text),
    m_itemHeight(ITEM_HEIGHT)
{

}


ItemInfo::ItemInfo(const QString &strName, const QString &datetime, const QString &pixmap, const QString &text, const QString &strSize, const quint8 &orientation, const quint8 &msgType) :
    m_strName(strName),
    m_strDatetime(datetime),
    m_strPixmap(pixmap),
    m_strText(text),
    m_strSize(strSize),
    m_orientation(orientation),
    m_msgType(msgType),
    m_itemHeight(ITEM_HEIGHT)
{

}

ItemInfo::~ItemInfo() {

}

void ItemInfo::SetName(const QString &text)
{
        m_strName = text;
}

QString ItemInfo::GetName() const
{
    return m_strName;
}

void ItemInfo::SetDatetime(const QString &text)
{
    m_strDatetime = text;
}

QString ItemInfo::GetDatetime() const
{
    return m_strDatetime;
}

void ItemInfo::SetHeadPixmap(const QString &pixmap)
{
    m_strPixmap = pixmap;
}

QString ItemInfo::GetStrPixmap() const {
    return m_strPixmap;
}


void ItemInfo::SetText(const QString &text)
{
    m_strText = text;
}

QString ItemInfo::GetText() const
{
    return m_strText;
}

void ItemInfo::SetFileSizeString(const QString &strSize)
{
    m_strSize = strSize;
}

QString ItemInfo::GetFileSizeString() const
{
    return m_strSize;
}

quint8 ItemInfo::GetOrientation() const
{
    return m_orientation;
}

void ItemInfo::SetOrientation(quint8 orientation)
{
    m_orientation = orientation;
}

quint8 ItemInfo::GetMsgType() const
{
    return m_msgType;
}

void ItemInfo::SetMsgType(const quint8 &msgType)
{
    m_msgType = msgType;
}

qreal ItemInfo::GetItemHeight() const
{
    return m_itemHeight;
}

void ItemInfo::SetItemHeight(qreal itemHeight)
{
    m_itemHeight = itemHeight;
}

QRectF ItemInfo::GetBobbleRect() const
{
    return m_bobbleRect;
}

void ItemInfo::SetBobbleRect(const QRectF &bobbleRect)
{
    m_bobbleRect = bobbleRect;
}
