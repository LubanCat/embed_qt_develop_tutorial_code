/****************************************************************************
** Meta object code from reading C++ file 'camerawidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Camera/src/camerawidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camerawidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CameraWidget_t {
    QByteArrayData data[18];
    char stringdata0[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CameraWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CameraWidget_t qt_meta_stringdata_CameraWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CameraWidget"
QT_MOC_LITERAL(1, 13, 10), // "on_timeout"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "on_takePicture"
QT_MOC_LITERAL(4, 40, 17), // "on_clieckBackhome"
QT_MOC_LITERAL(5, 58, 15), // "readyForCapture"
QT_MOC_LITERAL(6, 74, 5), // "ready"
QT_MOC_LITERAL(7, 80, 10), // "imageSaved"
QT_MOC_LITERAL(8, 91, 2), // "id"
QT_MOC_LITERAL(9, 94, 8), // "fileName"
QT_MOC_LITERAL(10, 103, 20), // "processCapturedImage"
QT_MOC_LITERAL(11, 124, 9), // "requestId"
QT_MOC_LITERAL(12, 134, 3), // "img"
QT_MOC_LITERAL(13, 138, 19), // "displayCaptureError"
QT_MOC_LITERAL(14, 158, 26), // "QCameraImageCapture::Error"
QT_MOC_LITERAL(15, 185, 5), // "error"
QT_MOC_LITERAL(16, 191, 11), // "errorString"
QT_MOC_LITERAL(17, 203, 18) // "displayCameraError"

    },
    "CameraWidget\0on_timeout\0\0on_takePicture\0"
    "on_clieckBackhome\0readyForCapture\0"
    "ready\0imageSaved\0id\0fileName\0"
    "processCapturedImage\0requestId\0img\0"
    "displayCaptureError\0QCameraImageCapture::Error\0"
    "error\0errorString\0displayCameraError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CameraWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    1,   57,    2, 0x08 /* Private */,
       7,    2,   60,    2, 0x08 /* Private */,
      10,    2,   65,    2, 0x08 /* Private */,
      13,    3,   70,    2, 0x08 /* Private */,
      17,    0,   77,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    8,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::QImage,   11,   12,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 14, QMetaType::QString,    8,   15,   16,
    QMetaType::Void,

       0        // eod
};

void CameraWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CameraWidget *_t = static_cast<CameraWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_timeout(); break;
        case 1: _t->on_takePicture(); break;
        case 2: _t->on_clieckBackhome(); break;
        case 3: _t->readyForCapture((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->imageSaved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->processCapturedImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QImage(*)>(_a[2]))); break;
        case 6: _t->displayCaptureError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QCameraImageCapture::Error(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 7: _t->displayCameraError(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCameraImageCapture::Error >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CameraWidget::staticMetaObject = {
    { &QtAnimationWidget::staticMetaObject, qt_meta_stringdata_CameraWidget.data,
      qt_meta_data_CameraWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CameraWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CameraWidget.stringdata0))
        return static_cast<void*>(this);
    return QtAnimationWidget::qt_metacast(_clname);
}

int CameraWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtAnimationWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
