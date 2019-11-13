/****************************************************************************
** Meta object code from reading C++ file 'capturewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../capturewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'capturewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CaptureWidget_t {
    QByteArrayData data[7];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CaptureWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CaptureWidget_t qt_meta_stringdata_CaptureWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CaptureWidget"
QT_MOC_LITERAL(1, 14, 9), // "startSlot"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 8), // "stopSlot"
QT_MOC_LITERAL(4, 34, 8), // "saveSlot"
QT_MOC_LITERAL(5, 43, 8), // "rtmpSlot"
QT_MOC_LITERAL(6, 52, 16) // "receiveImageSlot"

    },
    "CaptureWidget\0startSlot\0\0stopSlot\0"
    "saveSlot\0rtmpSlot\0receiveImageSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CaptureWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    1,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,    2,

       0        // eod
};

void CaptureWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CaptureWidget *_t = static_cast<CaptureWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startSlot(); break;
        case 1: _t->stopSlot(); break;
        case 2: _t->saveSlot(); break;
        case 3: _t->rtmpSlot(); break;
        case 4: _t->receiveImageSlot((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CaptureWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CaptureWidget.data,
      qt_meta_data_CaptureWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CaptureWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CaptureWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CaptureWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "BaseApp"))
        return static_cast< BaseApp*>(this);
    return QWidget::qt_metacast(_clname);
}

int CaptureWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
