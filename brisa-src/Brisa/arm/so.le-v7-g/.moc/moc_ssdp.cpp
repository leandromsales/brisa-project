/****************************************************************************
** Meta object code from reading C++ file 'ssdp.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/shared/ssdp/ssdp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ssdp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SSDP[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x05,
      31,    5,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,    5,    5,    5, 0x08,
      88,   70,    5,    5, 0x0a,
     130,  122,    5,    5, 0x2a,
     161,  156,    5,    5, 0x2a,
     179,    5,    5,    5, 0x2a,

       0        // eod
};

static const char qt_meta_stringdata_SSDP[] = {
    "SSDP\0\0messageReceived(QString)\0"
    "setupEvent(QString)\0datagramReceived()\0"
    "type,mx,userAgent\0discover(QString,QString,QString)\0"
    "type,mx\0discover(QString,QString)\0"
    "type\0discover(QString)\0discover()\0"
};

void SSDP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SSDP *_t = static_cast<SSDP *>(_o);
        switch (_id) {
        case 0: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->setupEvent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->datagramReceived(); break;
        case 3: _t->discover((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: _t->discover((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->discover((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->discover(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SSDP::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SSDP::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SSDP,
      qt_meta_data_SSDP, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SSDP::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SSDP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SSDP::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SSDP))
        return static_cast<void*>(const_cast< SSDP*>(this));
    return QObject::qt_metacast(_clname);
}

int SSDP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SSDP::messageReceived(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SSDP::setupEvent(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
