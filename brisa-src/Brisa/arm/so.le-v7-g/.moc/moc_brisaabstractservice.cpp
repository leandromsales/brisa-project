/****************************************************************************
** Meta object code from reading C++ file 'brisaabstractservice.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/brisaabstractservice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'brisaabstractservice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Brisa__BrisaAbstractService[] = {

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
      50,   29,   28,   28, 0x05,
     116,   92,   28,   28, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Brisa__BrisaAbstractService[] = {
    "Brisa::BrisaAbstractService\0\0"
    "arguments,methodName\0"
    "requestFinished(BrisaOutArgument,QString)\0"
    "errorMessage,methodName\0"
    "requestError(QString,QString)\0"
};

void Brisa::BrisaAbstractService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BrisaAbstractService *_t = static_cast<BrisaAbstractService *>(_o);
        switch (_id) {
        case 0: _t->requestFinished((*reinterpret_cast< BrisaOutArgument(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->requestError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Brisa::BrisaAbstractService::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Brisa::BrisaAbstractService::staticMetaObject = {
    { &WebService::staticMetaObject, qt_meta_stringdata_Brisa__BrisaAbstractService,
      qt_meta_data_Brisa__BrisaAbstractService, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Brisa::BrisaAbstractService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Brisa::BrisaAbstractService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Brisa::BrisaAbstractService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Brisa__BrisaAbstractService))
        return static_cast<void*>(const_cast< BrisaAbstractService*>(this));
    return WebService::qt_metacast(_clname);
}

int Brisa::BrisaAbstractService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Brisa::BrisaAbstractService::requestFinished(BrisaOutArgument _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Brisa::BrisaAbstractService::requestError(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
