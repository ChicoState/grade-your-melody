/****************************************************************************
** Meta object code from reading C++ file 'GridController.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../core/GridController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GridController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GridController_t {
    const uint offsetsAndSize[32];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_GridController_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_GridController_t qt_meta_stringdata_GridController = {
    {
QT_MOC_LITERAL(0, 14), // "GridController"
QT_MOC_LITERAL(15, 11), // "beatChanged"
QT_MOC_LITERAL(27, 0), // ""
QT_MOC_LITERAL(28, 4), // "beat"
QT_MOC_LITERAL(33, 14), // "setExpectedRow"
QT_MOC_LITERAL(48, 5), // "index"
QT_MOC_LITERAL(54, 3), // "row"
QT_MOC_LITERAL(58, 10), // "accidental"
QT_MOC_LITERAL(69, 11), // "expectedRow"
QT_MOC_LITERAL(81, 18), // "expectedAccidental"
QT_MOC_LITERAL(100, 7), // "setNote"
QT_MOC_LITERAL(108, 9), // "clearBeat"
QT_MOC_LITERAL(118, 7), // "hasNote"
QT_MOC_LITERAL(126, 5), // "score"
QT_MOC_LITERAL(132, 14), // "incorrectBeats"
QT_MOC_LITERAL(147, 13) // "totalExpected"

    },
    "GridController\0beatChanged\0\0beat\0"
    "setExpectedRow\0index\0row\0accidental\0"
    "expectedRow\0expectedAccidental\0setNote\0"
    "clearBeat\0hasNote\0score\0incorrectBeats\0"
    "totalExpected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GridController[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    1 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       4,    3,   83,    2, 0x02,    3 /* Public */,
       4,    2,   90,    2, 0x22,    7 /* Public | MethodCloned */,
       8,    1,   95,    2, 0x102,   10 /* Public | MethodIsConst  */,
       9,    1,   98,    2, 0x102,   12 /* Public | MethodIsConst  */,
      10,    3,  101,    2, 0x02,   14 /* Public */,
      11,    1,  108,    2, 0x02,   18 /* Public */,
      12,    2,  111,    2, 0x102,   20 /* Public | MethodIsConst  */,
      13,    0,  116,    2, 0x102,   23 /* Public | MethodIsConst  */,
      14,    0,  117,    2, 0x102,   24 /* Public | MethodIsConst  */,
      15,    0,  118,    2, 0x102,   25 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // methods: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    5,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Int, QMetaType::Int,    5,
    QMetaType::Int, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    6,    7,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Bool, QMetaType::Int, QMetaType::Int,    3,    6,
    QMetaType::Int,
    QMetaType::QVariantList,
    QMetaType::Int,

       0        // eod
};

void GridController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GridController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->beatChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->setExpectedRow((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 2: _t->setExpectedRow((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: { int _r = _t->expectedRow((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: { int _r = _t->expectedAccidental((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->setNote((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 6: _t->clearBeat((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: { bool _r = _t->hasNote((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->score();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { QVariantList _r = _t->incorrectBeats();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->totalExpected();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GridController::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GridController::beatChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject GridController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_GridController.offsetsAndSize,
    qt_meta_data_GridController,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_GridController_t
, QtPrivate::TypeAndForceComplete<GridController, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>

, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QVariantList, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>

>,
    nullptr
} };


const QMetaObject *GridController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GridController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GridController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GridController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void GridController::beatChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
