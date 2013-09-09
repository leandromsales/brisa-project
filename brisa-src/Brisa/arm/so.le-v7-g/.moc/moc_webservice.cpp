/****************************************************************************
** Meta object code from reading C++ file 'webservice.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/shared/webserver/webservice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'webservice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Brisa__WebService[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   19,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Brisa__WebService[] = {
    "Brisa::WebService\0\0,session\0"
    "genericRequestReceived(HttpRequest,WebserverSession*)\0"
};

void Brisa::WebService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WebService *_t = static_cast<WebService *>(_o);
        switch (_id) {
        case 0: _t->genericRequestReceived((*reinterpret_cast< HttpRequest(*)>(_a[1])),(*reinterpret_cast< WebserverSession*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Brisa::WebService::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Brisa::WebService::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Brisa__WebService,
      qt_meta_data_Brisa__WebService, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Brisa::WebService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Brisa::WebService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Brisa::WebService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Brisa__WebService))
        return static_cast<void*>(const_cast< WebService*>(this));
    return QObject::qt_metacast(_clname);
}

int Brisa::WebService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Brisa::WebService::genericRequestReceived(HttpRequest _t1, WebserverSession * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
