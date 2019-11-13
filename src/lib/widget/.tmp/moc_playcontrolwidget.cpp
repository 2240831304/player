/****************************************************************************
** Meta object code from reading C++ file 'playcontrolwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../playcontrolwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playcontrolwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_eink__PlayControlWidget_t {
    QByteArrayData data[7];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_eink__PlayControlWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_eink__PlayControlWidget_t qt_meta_stringdata_eink__PlayControlWidget = {
    {
QT_MOC_LITERAL(0, 0, 23), // "eink::PlayControlWidget"
QT_MOC_LITERAL(1, 24, 19), // "playStateClickedSig"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 11), // "nextPlaySig"
QT_MOC_LITERAL(4, 57, 11), // "lastPlaySig"
QT_MOC_LITERAL(5, 69, 17), // "changePlayTimePos"
QT_MOC_LITERAL(6, 87, 20) // "playStateClickedSlot"

    },
    "eink::PlayControlWidget\0playStateClickedSig\0"
    "\0nextPlaySig\0lastPlaySig\0changePlayTimePos\0"
    "playStateClickedSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_eink__PlayControlWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       3,    0,   42,    2, 0x06 /* Public */,
       4,    0,   43,    2, 0x06 /* Public */,
       5,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void eink::PlayControlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlayControlWidget *_t = static_cast<PlayControlWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->playStateClickedSig((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->nextPlaySig(); break;
        case 2: _t->lastPlaySig(); break;
        case 3: _t->changePlayTimePos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->playStateClickedSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (PlayControlWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayControlWidget::playStateClickedSig)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (PlayControlWidget::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayControlWidget::nextPlaySig)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (PlayControlWidget::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayControlWidget::lastPlaySig)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (PlayControlWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayControlWidget::changePlayTimePos)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject eink::PlayControlWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_eink__PlayControlWidget.data,
      qt_meta_data_eink__PlayControlWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *eink::PlayControlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *eink::PlayControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_eink__PlayControlWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int eink::PlayControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void eink::PlayControlWidget::playStateClickedSig(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void eink::PlayControlWidget::nextPlaySig()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void eink::PlayControlWidget::lastPlaySig()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void eink::PlayControlWidget::changePlayTimePos(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
