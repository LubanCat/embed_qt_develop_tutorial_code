#include <QApplication>

#include <QDebug>
#include <QWidget>

#include <QStack>
#include <QQueue>
#include <QSet>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /**************************************************************
     *
     *                      QList
     *
     * ************************************************************/

    QList<int> integerList ={1,2,3,4,5,6,7,8,9};
    QList<QChar> dateList;

    QList<QString> list = {"one", "two", "three"};
    list << "four" << "five";

    if (list[0] == "one")
        list[0] = "Robert";

    for (int i = 0; i< list.size(); ++i) {
        if (list.at(i)== "three")
            qDebug() << "Found three at position " << i;
    }

    int index = list.indexOf("three");
    if (index != -1)
       qDebug() << "First occurrence of Jane is at position " << index;

    QList<QWidget *> widgetlist;
    QWidget *w1 =new QWidget();
    QWidget *w2 =new QWidget();
    widgetlist.append(w1);
    widgetlist.append(w2);
    while (!widgetlist.isEmpty())
        delete widgetlist.takeFirst();

    /**************************************************************
     *
     *                      QVector
     *
     * ************************************************************/
    QVector<int> integerVector={1,2,3,4,5,6,7,8,9};
    QVector<QString> stringVector;
    QVector<QString> vector1(200);
    QVector<QString> vector2(200, "Pass");
    QVector<QString> vector = {"one", "two", "three"};
    if (vector[0] == "one")
        vector[0] = "Elizabeth";

    for (int i = 0; i< vector.size();++i) {
        if (vector.at(i) == "three")
            qDebug() << "Found three at position " << i ;
    }

    index = vector.indexOf("two");
    if (index != -1)
        qDebug() << "First occurrence of two is at position " << index;

    /**************************************************************
     *
     *                      QStack
     *
     * ************************************************************/
    QStack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);

    while (!stack.isEmpty())
       qDebug() << stack.pop();

    /**************************************************************
     *
     *                      QQueue
     *
     * ************************************************************/
    QQueue<int> queue;;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    while (!queue.isEmpty())
        qDebug() << queue.dequeue();

    /**************************************************************
     *
     *                      QSet
     *
     * ************************************************************/
    QSet<QString> set;

    set.insert("one");
    set.insert("three");
    set.insert("seven");
    set  << "twelve" << "fifteen" << "nineteen";

    if (set.contains("fifteen"))
         qDebug()<<"set contains fifteen";

    QSet<QWidget *> setwidget;
    QWidget *widget1 =new QWidget();
    widget1->setObjectName("widget1");
    QWidget *widget2 =new QWidget();
    widget2->setObjectName("widget2");
    setwidget.insert(widget1);
    setwidget.insert(widget2);

    QSetIterator<QWidget *> setIterator_java(setwidget);
    while (setIterator_java.hasNext())
        qDebug() << setIterator_java.next()->objectName();

    QSet<QWidget *>::const_iterator setIterator_stl = setwidget.constBegin();
    while (setIterator_stl != setwidget.constEnd()) {
        qDebug()  << *setIterator_stl;
        QWidget *widget=*setIterator_stl;
        qDebug() << widget->objectName();
        delete  widget;
        ++setIterator_stl;
    }

    foreach (const QString &value, set)
        qDebug()  << value;

    /**************************************************************
     *
     *                      QMap
     *
     * ************************************************************/
    QMap<QString, int> map;
    map["one"] = 1;
    map["three"] = 3;
    map["seven"] = 7;
    map.insert("twelve", 12);

    int num1 = map["thirteen"];
    qDebug()<<num1;
    int num2 = map.value("thirteen");
    qDebug()<<num2;

    int timeout = 30;
    if (map.contains("TIMEOUT"))
        timeout = map.value("TIMEOUT");

    qDebug()<<timeout;


    QMap<int, QWidget *> mapwidget;
    mapwidget.insert(1,widget1);
    mapwidget.insert(2,widget2);

    for (int i = 0;i < 1000;i++) {
        if (mapwidget[i]->objectName() == "widget1")
            qDebug() << "Found widget1 at index " << i;
    }

    QMapIterator<QString, int> mapIterator_java(map);
    while (mapIterator_java.hasNext()) {
        mapIterator_java.next();
        qDebug() << mapIterator_java.key() << ": " << mapIterator_java.value();
    }

    QMap<QString, int>::const_iterator mapIterator_stl = map.constBegin();
    while (mapIterator_stl != map.constEnd()) {
        qDebug() << mapIterator_stl.key() << ": " << mapIterator_stl.value();
        ++mapIterator_stl;
    }

    QList<int> values = map.values("twelve");
    for (int i = 0; i < values.size(); ++i)
        qDebug() << values.at(i);

    QMap<QString, int>::iterator i = map.find("three");
    while (i != map.end() && i.key() == "three") {
        qDebug() << i.value();
        ++i;
    }

    foreach (int value, map)
        qDebug() << value;

    /**************************************************************
     *
     *                      QMultiMap
     *
     * ************************************************************/
    QMultiMap<QString, int> map1, map2, map3;

    map1.insert("plenty", 100);
    map1.insert("plenty", 2000);
    qDebug()<<map1.size();

    map2.insert("plenty", 5000);
    qDebug()<<map2.size();

    map3 = map1 + map2;
    qDebug()<<map3.size();

    QList<int> valueslist = map.values("plenty");
    for (int i = 0; i< values.size(); ++i)
        qDebug() << values.at(i);

    QMultiMap<QString, int>::iterator iterator_multimap = map2.find("plenty");
    while (iterator_multimap != map2.end()&& iterator_multimap.key()== "plenty"){
        qDebug() << iterator_multimap.value();
        ++i;;
    }

    /**************************************************************
     *
     *                      QHash
     *
     * ************************************************************/
    QHash<QString, int> hash;
    hash["one"] = 1;
    hash["three"] = 3;
    hash["seven"] = 7;
    hash.insert("twelve", 12);
    qDebug() << hash["thirteen"];
    qDebug() << hash.value("thirteen");

    timeout = 30;
    if (hash.contains("TIMEOUT"))
        qDebug() << hash.value("TIMEOUT");

    qDebug() << hash.value("TIMEOUT", 30);

    QHash<int, QWidget *> hashwidget;
    hashwidget.insert(1,widget1);
    hashwidget.insert(2,widget2);
    for (int i = 0;i < 1000;i++) {
        if (hashwidget[i]->objectName() == "widget1")
            qDebug() << "Found widget1 at index " << i;
    }

    QHashIterator<QString, int> hashIterator_java(hash);
    while (hashIterator_java.hasNext()) {
        hashIterator_java.next();
        qDebug() << hashIterator_java.key() << ": " << hashIterator_java.value();
    }

    QHash<QString, int>::const_iterator hashIterator_stl = hash.constBegin();
    while (hashIterator_stl != hash.constEnd()) {
        qDebug() << hashIterator_stl.key() << ": " << hashIterator_stl.value();
        ++mapIterator_stl;
    }

    foreach (int value, hash)
        qDebug() << value;

    /**************************************************************
     *
     *                      QMultiHash
     *
     * ************************************************************/
    QMultiHash<QString, int> hash1, hash2, hash3;

    hash1.insert("plenty", 100);
    hash1.insert("plenty", 2000);
    qDebug() << hash1.size();

    hash2.insert("plenty", 5000);
    qDebug() << hash2.size();

    hash3 = hash1 + hash2;
    qDebug() << hash3.size();

    QList<int> hashvalues = hash1.values("plenty");
    for (int i = 0; i < values.size(); ++i)
        qDebug() << values.at(i);

    QMultiHash<QString, int>::iterator iterator = hash1.find("plenty");
    while (iterator!= hash1.end() && iterator.key() == "plenty") {
        qDebug() << i.value();
        ++i;
    }

    /**************************************************************
     *
     *                      java迭代器
     *
     * ************************************************************/
    QList<QString> listiterator;
    listiterator << "A" << "B" << "C" << "D";

    QListIterator<QString> listiterator_java1(listiterator);
    while (listiterator_java1.hasNext())
        qDebug() << listiterator_java1.next();

    QListIterator<QString> listiterator_java2(listiterator);
    listiterator_java2.toBack();
    while (listiterator_java2.hasPrevious())
        qDebug() << listiterator_java2.previous();

    QList<int> mutableiterator;
    mutableiterator << 1 << 2 << 3 << 4;

    QMutableListIterator<int> mutableiterator_java1(mutableiterator);
    while (mutableiterator_java1.hasNext()) {
        if (mutableiterator_java1.next() % 2 != 0)
            mutableiterator_java1.remove();
    }

    QMutableListIterator<int> mutableiterator_java2(mutableiterator);
    mutableiterator_java2.toBack();
    while (mutableiterator_java2.hasPrevious()) {
        if (mutableiterator_java2.previous() % 2 != 0)
            mutableiterator_java2.remove();
    }

    QMutableListIterator<int> mutableiterator_java3(mutableiterator);
    while (mutableiterator_java3.hasNext()) {
        if (mutableiterator_java3.next() < 128)
            mutableiterator_java3.setValue(128);
    }

    QMutableListIterator<int> mutableiterator_java4(mutableiterator);
    while (mutableiterator_java4.hasNext())
        mutableiterator_java4.next() *= 2;

    QMap<QString, QString> mapiterator;
    mapiterator.insert("Paris", "France");
    mapiterator.insert("Guatemala City", "Guatemala");
    mapiterator.insert("Mexico City", "Mexico");
    mapiterator.insert("Moscow", "Russia");

    QMutableMapIterator<QString, QString> mutableiterator_java5(mapiterator);
    while (mutableiterator_java5.hasNext()) {
        if (mutableiterator_java5.next().key().endsWith("City"))
            mutableiterator_java5.remove();
    }

    QMap<int, QWidget *> mapiterator1;
    QHash<int, QWidget *> hashiterator;

    QMapIterator<int, QWidget *> mapiterator_java5(mapiterator1);
    while (mapiterator_java5.hasNext()) {
        mapiterator_java5.next();
        hash.insert(i.key(), i.value());
    }

    /**************************************************************
     *
     *                      stl迭代器
     *
     * ************************************************************/
    QList<QString> listiteratorstl1;
    list << "A" << "B" << "C" << "D";

    QList<QString>::iterator listindex1;
    for (listindex1 = listiteratorstl1.begin(); listindex1 != listiteratorstl1.end(); ++listindex1)
        *listindex1 = (*listindex1).toLower();

    QList<QString> listiteratorstl2;
    list << "A" << "B" << "C" << "D";

    QList<QString>::reverse_iterator listindex2;
    for (listindex2 = listiteratorstl2.rbegin(); listindex2 != listiteratorstl2.rend(); ++listindex2){
        *listindex2 = listindex2->toLower();
    }

    QList<QString>::const_iterator listindex3;
    for (listindex3 = list.constBegin(); listindex3 != list.constEnd(); ++listindex3)
        qDebug() << *listindex3;

    QMap<int, int> mapiteratorstl;
    QMap<int, int>::const_iterator mapindex;
    for (mapindex = mapiteratorstl.constBegin(); mapindex != mapiteratorstl.constEnd(); ++mapindex)
        qDebug() << mapindex.key() << ':' << mapindex.value();

    /**************************************************************
     *
     *                      foreach 关键字
     *
     * ************************************************************/
    QList<QString> strlist;
    QString str;
    foreach (str, strlist)
        qDebug() << str;

    foreach (const QString &str, strlist)
        qDebug() << str;

    foreach (const QString &str, strlist) {
        if (str.isEmpty())
            break;
        qDebug() << str;
    }

    QListIterator<QString> strindex(strlist);
    while (strindex.hasNext())
        qDebug() << strindex.next();

    QMap<QString, int> strmap;

    foreach (const QString &str, strmap.keys())
        qDebug() << str << ':' << strmap.value(str);

    QMultiMap<QString, int> strmultimap;
    foreach (const QString &str, strmultimap.uniqueKeys()) {
        foreach (int i, strmultimap.values(str))
            qDebug() << str << ':' << i;
    }

    return a.exec();
}
