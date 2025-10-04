/****************************************************************************
** Meta object code from reading C++ file 'transmitter.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../menus/transmitter.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'transmitter.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_transmitter_t {
    uint offsetsAndSizes[28];
    char stringdata0[12];
    char stringdata1[27];
    char stringdata2[1];
    char stringdata3[23];
    char stringdata4[16];
    char stringdata5[9];
    char stringdata6[12];
    char stringdata7[23];
    char stringdata8[6];
    char stringdata9[23];
    char stringdata10[23];
    char stringdata11[21];
    char stringdata12[22];
    char stringdata13[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_transmitter_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_transmitter_t qt_meta_stringdata_transmitter = {
    {
        QT_MOC_LITERAL(0, 11),  // "transmitter"
        QT_MOC_LITERAL(12, 26),  // "OnEquipmentSelectorChanged"
        QT_MOC_LITERAL(39, 0),  // ""
        QT_MOC_LITERAL(40, 22),  // "OnLabelSelectorChanged"
        QT_MOC_LITERAL(63, 15),  // "_selectorindexd"
        QT_MOC_LITERAL(79, 8),  // "addLabel"
        QT_MOC_LITERAL(88, 11),  // "removeLabel"
        QT_MOC_LITERAL(100, 22),  // "onArinc_parity_bitRate"
        QT_MOC_LITERAL(123, 5),  // "index"
        QT_MOC_LITERAL(129, 22),  // "onSelectSaveConfigFile"
        QT_MOC_LITERAL(152, 22),  // "onSelectLoadConfigFile"
        QT_MOC_LITERAL(175, 20),  // "onTransmitterEnabled"
        QT_MOC_LITERAL(196, 21),  // "onTransmitterDisabled"
        QT_MOC_LITERAL(218, 12)   // "onLoadConfig"
    },
    "transmitter",
    "OnEquipmentSelectorChanged",
    "",
    "OnLabelSelectorChanged",
    "_selectorindexd",
    "addLabel",
    "removeLabel",
    "onArinc_parity_bitRate",
    "index",
    "onSelectSaveConfigFile",
    "onSelectLoadConfigFile",
    "onTransmitterEnabled",
    "onTransmitterDisabled",
    "onLoadConfig"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_transmitter[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x0a,    1 /* Public */,
       3,    1,   77,    2, 0x0a,    3 /* Public */,
       5,    1,   80,    2, 0x0a,    5 /* Public */,
       6,    1,   83,    2, 0x0a,    7 /* Public */,
       7,    1,   86,    2, 0x0a,    9 /* Public */,
       9,    1,   89,    2, 0x0a,   11 /* Public */,
      10,    1,   92,    2, 0x0a,   13 /* Public */,
      11,    1,   95,    2, 0x0a,   15 /* Public */,
      12,    1,   98,    2, 0x0a,   17 /* Public */,
      13,    1,  101,    2, 0x0a,   19 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

Q_CONSTINIT const QMetaObject transmitter::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_transmitter.offsetsAndSizes,
    qt_meta_data_transmitter,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_transmitter_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<transmitter, std::true_type>,
        // method 'OnEquipmentSelectorChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'OnLabelSelectorChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'addLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'removeLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onArinc_parity_bitRate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSelectSaveConfigFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onSelectLoadConfigFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onTransmitterEnabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onTransmitterDisabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onLoadConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void transmitter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<transmitter *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->OnEquipmentSelectorChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->OnLabelSelectorChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->addLabel((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->removeLabel((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->onArinc_parity_bitRate((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onSelectSaveConfigFile((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->onSelectLoadConfigFile((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->onTransmitterEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->onTransmitterDisabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->onLoadConfig((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *transmitter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *transmitter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_transmitter.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int transmitter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
