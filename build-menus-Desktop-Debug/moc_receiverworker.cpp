/****************************************************************************
** Meta object code from reading C++ file 'receiverworker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../menus/receiverworker.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receiverworker.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_ReceiverWorker_t {
    uint offsetsAndSizes[46];
    char stringdata0[15];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[6];
    char stringdata5[6];
    char stringdata6[6];
    char stringdata7[8];
    char stringdata8[5];
    char stringdata9[6];
    char stringdata10[17];
    char stringdata11[14];
    char stringdata12[16];
    char stringdata13[18];
    char stringdata14[12];
    char stringdata15[7];
    char stringdata16[9];
    char stringdata17[4];
    char stringdata18[8];
    char stringdata19[7];
    char stringdata20[6];
    char stringdata21[17];
    char stringdata22[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ReceiverWorker_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ReceiverWorker_t qt_meta_stringdata_ReceiverWorker = {
    {
        QT_MOC_LITERAL(0, 14),  // "ReceiverWorker"
        QT_MOC_LITERAL(15, 12),  // "setLabelData"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 8),  // "uint32_t"
        QT_MOC_LITERAL(38, 5),  // "deiId"
        QT_MOC_LITERAL(44, 5),  // "deiCh"
        QT_MOC_LITERAL(50, 5),  // "str_t"
        QT_MOC_LITERAL(56, 7),  // "labelId"
        QT_MOC_LITERAL(64, 4),  // "rate"
        QT_MOC_LITERAL(69, 5),  // "value"
        QT_MOC_LITERAL(75, 16),  // "idleLabelCleaner"
        QT_MOC_LITERAL(92, 13),  // "evalDataRates"
        QT_MOC_LITERAL(106, 15),  // "enableEvalrates"
        QT_MOC_LITERAL(122, 17),  // "enableIdleCleaner"
        QT_MOC_LITERAL(140, 11),  // "receiveTask"
        QT_MOC_LITERAL(152, 6),  // "update"
        QT_MOC_LITERAL(159, 8),  // "uint8_t&"
        QT_MOC_LITERAL(168, 3),  // "dei"
        QT_MOC_LITERAL(172, 7),  // "chanell"
        QT_MOC_LITERAL(180, 6),  // "float&"
        QT_MOC_LITERAL(187, 5),  // "_rate"
        QT_MOC_LITERAL(193, 16),  // "std::bitset<32>&"
        QT_MOC_LITERAL(210, 13)   // "arincBitsData"
    },
    "ReceiverWorker",
    "setLabelData",
    "",
    "uint32_t",
    "deiId",
    "deiCh",
    "str_t",
    "labelId",
    "rate",
    "value",
    "idleLabelCleaner",
    "evalDataRates",
    "enableEvalrates",
    "enableIdleCleaner",
    "receiveTask",
    "update",
    "uint8_t&",
    "dei",
    "chanell",
    "float&",
    "_rate",
    "std::bitset<32>&",
    "arincBitsData"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ReceiverWorker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    5,   56,    2, 0x06,    1 /* Public */,
      10,    0,   67,    2, 0x06,    7 /* Public */,
      11,    0,   68,    2, 0x06,    8 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    0,   69,    2, 0x0a,    9 /* Public */,
      13,    0,   70,    2, 0x0a,   10 /* Public */,
      14,    0,   71,    2, 0x0a,   11 /* Public */,
      15,    4,   72,    2, 0x0a,   12 /* Public */,

 // signals: parameters
    QMetaType::Bool, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 6, QMetaType::Float, QMetaType::QVariant,    4,    5,    7,    8,    9,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 16, 0x80000000 | 19, 0x80000000 | 21,   17,   18,   20,   22,

       0        // eod
};

Q_CONSTINIT const QMetaObject ReceiverWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ReceiverWorker.offsetsAndSizes,
    qt_meta_data_ReceiverWorker,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ReceiverWorker_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ReceiverWorker, std::true_type>,
        // method 'setLabelData'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<str_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<const float &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        // method 'idleLabelCleaner'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'evalDataRates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'enableEvalrates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'enableIdleCleaner'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'receiveTask'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'update'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint8_t &, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint8_t &, std::false_type>,
        QtPrivate::TypeAndForceComplete<float &, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::bitset<32> &, std::false_type>
    >,
    nullptr
} };

void ReceiverWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ReceiverWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { bool _r = _t->setLabelData((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<str_t>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->idleLabelCleaner(); break;
        case 2: _t->evalDataRates(); break;
        case 3: _t->enableEvalrates(); break;
        case 4: _t->enableIdleCleaner(); break;
        case 5: _t->receiveTask(); break;
        case 6: _t->update((*reinterpret_cast< std::add_pointer_t<uint8_t&>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint8_t&>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<float&>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<std::bitset<32>&>>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = bool (ReceiverWorker::*)(uint32_t , uint32_t , str_t , const float & , const QVariant & );
            if (_t _q_method = &ReceiverWorker::setLabelData; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ReceiverWorker::*)();
            if (_t _q_method = &ReceiverWorker::idleLabelCleaner; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ReceiverWorker::*)();
            if (_t _q_method = &ReceiverWorker::evalDataRates; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *ReceiverWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReceiverWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ReceiverWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ReceiverWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
bool ReceiverWorker::setLabelData(uint32_t _t1, uint32_t _t2, str_t _t3, const float & _t4, const QVariant & _t5)
{
    bool _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t0))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}

// SIGNAL 1
void ReceiverWorker::idleLabelCleaner()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ReceiverWorker::evalDataRates()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
