/****************************************************************************
** Meta object code from reading C++ file 'receiver.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../menus/receiver.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receiver.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_Receiver_t {
    uint offsetsAndSizes[40];
    char stringdata0[9];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[27];
    char stringdata4[4];
    char stringdata5[11];
    char stringdata6[15];
    char stringdata7[6];
    char stringdata8[18];
    char stringdata9[19];
    char stringdata10[13];
    char stringdata11[9];
    char stringdata12[4];
    char stringdata13[3];
    char stringdata14[6];
    char stringdata15[8];
    char stringdata16[5];
    char stringdata17[6];
    char stringdata18[14];
    char stringdata19[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_Receiver_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_Receiver_t qt_meta_stringdata_Receiver = {
    {
        QT_MOC_LITERAL(0, 8),  // "Receiver"
        QT_MOC_LITERAL(9, 18),  // "OnArincDataUpdated"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 26),  // "OnEquipmentSelectorChanged"
        QT_MOC_LITERAL(56, 3),  // "_id"
        QT_MOC_LITERAL(60, 10),  // "UpdateTree"
        QT_MOC_LITERAL(71, 14),  // "on_SDI_bitRate"
        QT_MOC_LITERAL(86, 5),  // "index"
        QT_MOC_LITERAL(92, 17),  // "onReceiverEnabled"
        QT_MOC_LITERAL(110, 18),  // "onReceiverDisabled"
        QT_MOC_LITERAL(129, 12),  // "setLabelData"
        QT_MOC_LITERAL(142, 8),  // "uint32_t"
        QT_MOC_LITERAL(151, 3),  // "dei"
        QT_MOC_LITERAL(155, 2),  // "ch"
        QT_MOC_LITERAL(158, 5),  // "str_t"
        QT_MOC_LITERAL(164, 7),  // "labelId"
        QT_MOC_LITERAL(172, 4),  // "rate"
        QT_MOC_LITERAL(177, 5),  // "value"
        QT_MOC_LITERAL(183, 13),  // "evalDataRates"
        QT_MOC_LITERAL(197, 16)   // "idleLabelCleaner"
    },
    "Receiver",
    "OnArincDataUpdated",
    "",
    "OnEquipmentSelectorChanged",
    "_id",
    "UpdateTree",
    "on_SDI_bitRate",
    "index",
    "onReceiverEnabled",
    "onReceiverDisabled",
    "setLabelData",
    "uint32_t",
    "dei",
    "ch",
    "str_t",
    "labelId",
    "rate",
    "value",
    "evalDataRates",
    "idleLabelCleaner"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_Receiver[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    1,   69,    2, 0x0a,    2 /* Public */,
       5,    0,   72,    2, 0x0a,    4 /* Public */,
       6,    1,   73,    2, 0x0a,    5 /* Public */,
       8,    1,   76,    2, 0x0a,    7 /* Public */,
       9,    1,   79,    2, 0x0a,    9 /* Public */,
      10,    5,   82,    2, 0x0a,   11 /* Public */,
      18,    0,   93,    2, 0x0a,   17 /* Public */,
      19,    0,   94,    2, 0x0a,   18 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Bool, 0x80000000 | 11, 0x80000000 | 11, 0x80000000 | 14, QMetaType::Float, QMetaType::QVariant,   12,   13,   15,   16,   17,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Receiver::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Receiver.offsetsAndSizes,
    qt_meta_data_Receiver,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_Receiver_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Receiver, std::true_type>,
        // method 'OnArincDataUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'OnEquipmentSelectorChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'UpdateTree'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_SDI_bitRate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onReceiverEnabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onReceiverDisabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setLabelData'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<str_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<const float &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        // method 'evalDataRates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'idleLabelCleaner'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Receiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Receiver *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->OnArincDataUpdated(); break;
        case 1: _t->OnEquipmentSelectorChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->UpdateTree(); break;
        case 3: _t->on_SDI_bitRate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->onReceiverEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->onReceiverDisabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: { bool _r = _t->setLabelData((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<str_t>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->evalDataRates(); break;
        case 8: _t->idleLabelCleaner(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Receiver::*)();
            if (_t _q_method = &Receiver::OnArincDataUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *Receiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Receiver::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Receiver.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Receiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Receiver::OnArincDataUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
