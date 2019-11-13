/****************************************************************************
** Meta object code from reading C++ file 'audioplayerwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../audioplayerwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audioplayerwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AudioPlayerWidget_t {
    QByteArrayData data[6];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioPlayerWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioPlayerWidget_t qt_meta_stringdata_AudioPlayerWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "AudioPlayerWidget"
QT_MOC_LITERAL(1, 18, 17), // "doubleClickedsLOT"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 13), // "systemMessage"
QT_MOC_LITERAL(4, 51, 17), // "playNextMusicSlot"
QT_MOC_LITERAL(5, 69, 17) // "playLastMusicSlot"

    },
    "AudioPlayerWidget\0doubleClickedsLOT\0"
    "\0systemMessage\0playNextMusicSlot\0"
    "playLastMusicSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioPlayerWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       3,    2,   37,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AudioPlayerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioPlayerWidget *_t = static_cast<AudioPlayerWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->doubleClickedsLOT((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->systemMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->playNextMusicSlot(); break;
        case 3: _t->playLastMusicSlot(); break;
        default: ;
        }
    }
}

const QMetaObject AudioPlayerWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AudioPlayerWidget.data,
      qt_meta_data_AudioPlayerWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AudioPlayerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioPlayerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AudioPlayerWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "BaseApp"))
        return static_cast< BaseApp*>(this);
    return QWidget::qt_metacast(_clname);
}

int AudioPlayerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
