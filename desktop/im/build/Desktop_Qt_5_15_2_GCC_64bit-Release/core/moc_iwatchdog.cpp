/****************************************************************************
** Meta object code from reading C++ file 'iwatchdog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../core/interface/iwatchdog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'iwatchdog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_fl_common__IWatchDog_t {
    QByteArrayData data[10];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_fl_common__IWatchDog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_fl_common__IWatchDog_t qt_meta_stringdata_fl_common__IWatchDog = {
    {
QT_MOC_LITERAL(0, 0, 20), // "fl_common::IWatchDog"
QT_MOC_LITERAL(1, 21, 18), // "setFeeddogInterval"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 12), // "intervalSecs"
QT_MOC_LITERAL(4, 54, 10), // "setTimeout"
QT_MOC_LITERAL(5, 65, 4), // "secs"
QT_MOC_LITERAL(6, 70, 13), // "setFeedEnable"
QT_MOC_LITERAL(7, 84, 7), // "bEnable"
QT_MOC_LITERAL(8, 92, 9), // "feedCount"
QT_MOC_LITERAL(9, 102, 5) // "count"

    },
    "fl_common::IWatchDog\0setFeeddogInterval\0"
    "\0intervalSecs\0setTimeout\0secs\0"
    "setFeedEnable\0bEnable\0feedCount\0count"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_fl_common__IWatchDog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       1,    0,   52,    2, 0x26 /* Public | MethodCloned */,
       4,    1,   53,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x26 /* Public | MethodCloned */,
       6,    1,   57,    2, 0x06 /* Public */,
       6,    0,   60,    2, 0x26 /* Public | MethodCloned */,
       8,    1,   61,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,

       0        // eod
};

void fl_common::IWatchDog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<IWatchDog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setFeeddogInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setFeeddogInterval(); break;
        case 2: _t->setTimeout((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setTimeout(); break;
        case 4: _t->setFeedEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setFeedEnable(); break;
        case 6: _t->feedCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (IWatchDog::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IWatchDog::setFeeddogInterval)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (IWatchDog::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IWatchDog::setTimeout)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (IWatchDog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IWatchDog::setFeedEnable)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (IWatchDog::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IWatchDog::feedCount)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject fl_common::IWatchDog::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_fl_common__IWatchDog.data,
    qt_meta_data_fl_common__IWatchDog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *fl_common::IWatchDog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *fl_common::IWatchDog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_fl_common__IWatchDog.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IBase"))
        return static_cast< IBase*>(this);
    return QThread::qt_metacast(_clname);
}

int fl_common::IWatchDog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void fl_common::IWatchDog::setFeeddogInterval(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void fl_common::IWatchDog::setTimeout(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 4
void fl_common::IWatchDog::setFeedEnable(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 6
void fl_common::IWatchDog::feedCount(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
