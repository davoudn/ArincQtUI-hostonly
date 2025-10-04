/****************************************************************************
** Meta object code from reading C++ file 'TreeDataModel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../menus/TreeDataModel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TreeDataModel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_MyDataModel_t {
    uint offsetsAndSizes[20];
    char stringdata0[12];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[10];
    char stringdata5[5];
    char stringdata6[6];
    char stringdata7[20];
    char stringdata8[14];
    char stringdata9[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MyDataModel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MyDataModel_t qt_meta_stringdata_MyDataModel = {
    {
        QT_MOC_LITERAL(0, 11),  // "MyDataModel"
        QT_MOC_LITERAL(12, 12),  // "setLabelData"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 5),  // "str_t"
        QT_MOC_LITERAL(32, 9),  // "labelName"
        QT_MOC_LITERAL(42, 4),  // "rate"
        QT_MOC_LITERAL(47, 5),  // "value"
        QT_MOC_LITERAL(53, 19),  // "enableLayoutRefresh"
        QT_MOC_LITERAL(73, 13),  // "evalDataRates"
        QT_MOC_LITERAL(87, 16)   // "cleanTimeoutList"
    },
    "MyDataModel",
    "setLabelData",
    "",
    "str_t",
    "labelName",
    "rate",
    "value",
    "enableLayoutRefresh",
    "evalDataRates",
    "cleanTimeoutList"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MyDataModel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   38,    2, 0x0a,    1 /* Public */,
       7,    0,   45,    2, 0x0a,    5 /* Public */,
       8,    0,   46,    2, 0x0a,    6 /* Public */,
       9,    0,   47,    2, 0x0a,    7 /* Public */,

 // slots: parameters
    QMetaType::Bool, 0x80000000 | 3, QMetaType::Float, QMetaType::QVariant,    4,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MyDataModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractItemModel::staticMetaObject>(),
    qt_meta_stringdata_MyDataModel.offsetsAndSizes,
    qt_meta_data_MyDataModel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MyDataModel_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MyDataModel, std::true_type>,
        // method 'setLabelData'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<str_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<const float &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        // method 'enableLayoutRefresh'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'evalDataRates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cleanTimeoutList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MyDataModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyDataModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { bool _r = _t->setLabelData((*reinterpret_cast< std::add_pointer_t<str_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->enableLayoutRefresh(); break;
        case 2: _t->evalDataRates(); break;
        case 3: _t->cleanTimeoutList(); break;
        default: ;
        }
    }
}

const QMetaObject *MyDataModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyDataModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyDataModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractItemModel::qt_metacast(_clname);
}

int MyDataModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
