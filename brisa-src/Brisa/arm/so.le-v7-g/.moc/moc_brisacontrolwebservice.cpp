/****************************************************************************
** Meta object code from reading C++ file 'brisacontrolwebservice.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/device/brisacontrolwebservice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'brisacontrolwebservice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Brisa__BrisaControlWebService[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      47,   31,   30,   30, 0x05,
     113,  106,   30,   30, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Brisa__BrisaControlWebService[] = {
    "Brisa::BrisaControlWebService\0\0"
    "xml,args,server\0"
    "requestReceived(QString,BrisaInArgument,WebserverSession*)\0"
    "server\0invalidRequest(WebserverSession*)\0"
};

void Brisa::BrisaControlWebService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BrisaControlWebService *_t = static_cast<BrisaControlWebService *>(_o);
        switch (_id) {
        case 0: _t->requestReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< BrisaInArgument(*)>(_a[2])),(*reinterpret_cast< WebserverSession*(*)>(_a[3]))); break;
        case 1: _t->invalidRequest((*reinterpret_cast< WebserverSession*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Brisa::BrisaControlWebService::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Brisa::BrisaControlWebService::staticMetaObject = {
    { &WebService::staticMetaObject, qt_meta_stringdata_Brisa__BrisaControlWebService,
      qt_meta_data_Brisa__BrisaControlWebService, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Brisa::BrisaControlWebService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Brisa::BrisaControlWebService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Brisa::BrisaControlWebService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Brisa__BrisaControlWebService))
        return static_cast<void*>(const_cast< BrisaControlWebService*>(this));
    return WebService::qt_metacast(_clname);
}

int Brisa::BrisaControlWebService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WebService::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Brisa::BrisaControlWebService::requestReceived(QString _t1, BrisaInArgument _t2, WebserverSession * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Brisa::BrisaControlWebService::invalidRequest(WebserverSession * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
