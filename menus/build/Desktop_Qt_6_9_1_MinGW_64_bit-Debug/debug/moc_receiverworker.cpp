/****************************************************************************
** Meta object code from reading C++ file 'receiverworker.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../receiverworker.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receiverworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN14ReceiverWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto ReceiverWorker::qt_create_metaobjectdata<qt_meta_tag_ZN14ReceiverWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ReceiverWorker",
        "setLabelData",
        "",
        "uint32_t",
        "deiId",
        "deiCh",
        "str_t",
        "labelId",
        "rate",
        "QVariant",
        "value",
        "idleLabelCleaner",
        "evalDataRates",
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

    QtMocHelpers::UintData qt_methods {
        // Signal 'setLabelData'
        QtMocHelpers::SignalData<bool(uint32_t, uint32_t, str_t, const float &, const QVariant &)>(1, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 3, 5 }, { 0x80000000 | 6, 7 }, { QMetaType::Float, 8 },
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'idleLabelCleaner'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'evalDataRates'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'receiveTask'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'update'
        QtMocHelpers::SlotData<void(uint8_t &, uint8_t &, float &, std::bitset<32> &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 15, 16 }, { 0x80000000 | 15, 17 }, { 0x80000000 | 18, 19 }, { 0x80000000 | 20, 21 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ReceiverWorker, qt_meta_tag_ZN14ReceiverWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ReceiverWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ReceiverWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ReceiverWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ReceiverWorkerE_t>.metaTypes,
    nullptr
} };

void ReceiverWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ReceiverWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = _t->setLabelData((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<str_t>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->idleLabelCleaner(); break;
        case 2: _t->evalDataRates(); break;
        case 3: _t->receiveTask(); break;
        case 4: _t->update((*reinterpret_cast< std::add_pointer_t<uint8_t&>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint8_t&>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<float&>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<std::bitset<32>&>>(_a[4]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<bool (ReceiverWorker::*)(uint32_t , uint32_t , str_t , const float & , const QVariant & )>(_a, &ReceiverWorker::setLabelData, 0))
            return;
    }
}

const QMetaObject *ReceiverWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReceiverWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ReceiverWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ReceiverWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
bool ReceiverWorker::setLabelData(uint32_t _t1, uint32_t _t2, str_t _t3, const float & _t4, const QVariant & _t5)
{
    bool _t0{};
    QMetaObject::activate<bool>(this, &staticMetaObject, 0, std::addressof(_t0), _t1, _t2, _t3, _t4, _t5);
    return _t0;
}
QT_WARNING_POP
