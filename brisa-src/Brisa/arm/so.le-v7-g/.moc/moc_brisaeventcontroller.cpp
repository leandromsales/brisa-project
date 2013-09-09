/****************************************************************************
** Meta object code from reading C++ file 'brisaeventcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/device/brisaeventcontroller.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'brisaeventcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Brisa__BrisaEventController[] = {

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
      38,   29,   28,   28, 0x0a,
      91,   75,   28,   28, 0x0a,
     132,   75,   28,   28, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Brisa__BrisaEventController[] = {
    "Brisa::BrisaEventController\0\0variable\0"
    "variableChanged(BrisaStateVariable*)\0"
    "request,session\0"
    "subscribe(HttpRequest,WebserverSession*)\0"
    "unsubscribe(HttpRequest,WebserverSession*)\0"
};

void Brisa::BrisaEventController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BrisaEventController *_t = static_cast<BrisaEventController *>(_o);
        switch (_id) {
        case 0: _t->variableChanged((*reinterpret_cast< BrisaStateVariable*(*)>(_a[1]))); break;
        case 1: _t->subscribe((*reinterpret_cast< const HttpRequest(*)>(_a[1])),(*reinterpret_cast< WebserverSession*(*)>(_a[2]))); break;
        case 2: _t->unsubscribe((*reinterpret_cast< const HttpRequest(*)>(_a[1])),(*reinterpret_cast< WebserverSession*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Brisa::BrisaEventController::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Brisa::BrisaEventController::staticMetaObject = {
    { &WebService::staticMetaObject, qt_meta_stringdata_Brisa__BrisaEventController,
      qt_meta_data_Brisa__BrisaEventController, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Brisa::BrisaEventController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Brisa::BrisaEventController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Brisa::BrisaEventController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Brisa__BrisaEventController))
        return static_cast<void*>(const_cast< BrisaEventController*>(this));
    return WebService::qt_metacast(_clname);
}

int Brisa::BrisaEventController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WebService::qt_metacall(_c, _id, _a);
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
