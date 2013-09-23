/****************************************************************************
** Meta object code from reading C++ file 'MyService.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/MyService.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MyService.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyService[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      48,   29,   11,   10, 0x08,

 // methods: signature, parameters, type, tag, flags
     101,   29,   11,   10, 0x02,
     154,   29,   11,   10, 0x00,

       0        // eod
};

static const char qt_meta_stringdata_MyService[] = {
    "MyService\0\0BrisaOutArgument*\0"
    "inArguments,action\0"
    "actionName3(BrisaInArgument*const,BrisaAction*const)\0"
    "actionName1(BrisaInArgument*const,BrisaAction*const)\0"
    "actionName2(BrisaInArgument*const,BrisaAction*const)\0"
};

void MyService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MyService *_t = static_cast<MyService *>(_o);
        switch (_id) {
        case 0: { BrisaOutArgument* _r = _t->actionName3((*reinterpret_cast< BrisaInArgument*const(*)>(_a[1])),(*reinterpret_cast< BrisaAction*const(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< BrisaOutArgument**>(_a[0]) = _r; }  break;
        case 1: { BrisaOutArgument* _r = _t->actionName1((*reinterpret_cast< BrisaInArgument*const(*)>(_a[1])),(*reinterpret_cast< BrisaAction*const(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< BrisaOutArgument**>(_a[0]) = _r; }  break;
        case 2: { BrisaOutArgument* _r = _t->actionName2((*reinterpret_cast< BrisaInArgument*const(*)>(_a[1])),(*reinterpret_cast< BrisaAction*const(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< BrisaOutArgument**>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MyService::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MyService::staticMetaObject = {
    { &BrisaService::staticMetaObject, qt_meta_stringdata_MyService,
      qt_meta_data_MyService, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyService))
        return static_cast<void*>(const_cast< MyService*>(this));
    return BrisaService::qt_metacast(_clname);
}

int MyService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BrisaService::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
