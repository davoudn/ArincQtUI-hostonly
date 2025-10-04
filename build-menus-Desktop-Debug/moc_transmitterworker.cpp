/****************************************************************************
** Meta object code from reading C++ file 'transmitterworker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../menus/transmitterworker.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'transmitterworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_TransmitterWorker_t {
    uint offsetsAndSizes[16];
    char stringdata0[18];
    char stringdata1[9];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[3];
    char stringdata5[17];
    char stringdata6[31];
    char stringdata7[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_TransmitterWorker_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_TransmitterWorker_t qt_meta_stringdata_TransmitterWorker = {
    {
        QT_MOC_LITERAL(0, 17),  // "TransmitterWorker"
        QT_MOC_LITERAL(18, 8),  // "sendData"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 5),  // "char*"
        QT_MOC_LITERAL(34, 2),  // "ac"
        QT_MOC_LITERAL(37, 16),  // "taskTransmitData"
        QT_MOC_LITERAL(54, 30),  // "incrementLabelsDataRateCounter"
        QT_MOC_LITERAL(85, 17)   // "actionListCleaner"
    },
    "TransmitterWorker",
    "sendData",
    "",
    "char*",
    "ac",
    "taskTransmitData",
    "incrementLabelsDataRateCounter",
    "actionListCleaner"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_TransmitterWorker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   41,    2, 0x0a,    3 /* Public */,
       6,    0,   42,    2, 0x0a,    4 /* Public */,
       7,    0,   43,    2, 0x0a,    5 /* Public */,

 // signals: parameters
    QMetaType::Bool, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject TransmitterWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_TransmitterWorker.offsetsAndSizes,
    qt_meta_data_TransmitterWorker,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_TransmitterWorker_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TransmitterWorker, std::true_type>,
        // method 'sendData'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<char *, std::false_type>,
        // method 'taskTransmitData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'incrementLabelsDataRateCounter'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actionListCleaner'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void TransmitterWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TransmitterWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { bool _r = _t->sendData((*reinterpret_cast< std::add_pointer_t<char*>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->taskTransmitData(); break;
        case 2: _t->incrementLabelsDataRateCounter(); break;
        case 3: _t->actionListCleaner(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = bool (TransmitterWorker::*)(char * );
            if (_t _q_method = &TransmitterWorker::sendData; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *TransmitterWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TransmitterWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TransmitterWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TransmitterWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
bool TransmitterWorker::sendData(char * _t1)
{
    bool _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t0))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
