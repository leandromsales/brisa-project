/****************************************************************************
** Meta object code from reading C++ file 'brisassdpserver.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/shared/ssdp/brisassdpserver.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'brisassdpserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Brisa__BrisaSSDPServer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      47,   24,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     100,   23,   95,   23, 0x0a,
     112,   23,   23,   23, 0x0a,
     120,   23,   23,   23, 0x0a,
     163,  127,   23,   23, 0x0a,
     220,  213,   23,   23, 0x0a,
     307,  246,   23,   23, 0x0a,
     416,  387,   23,   23, 0x08,
     466,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Brisa__BrisaSSDPServer[] = {
    "Brisa::BrisaSSDPServer\0\0st,senderIp,senderPort\0"
    "msearchRequestReceived(QString,QString,quint16)\0"
    "bool\0isRunning()\0start()\0stop()\0"
    "usn,location,st,server,cacheControl\0"
    "doNotify(QString,QString,QString,QString,QString)\0"
    "usn,st\0doByeBye(QString,QString)\0"
    "senderIp,senderPort,cacheControl,date,location,server,st,usn\0"
    "respondMSearch(QString,quint16,QString,QString,QString,QString,QString"
    ",QString)\0"
    "datagram,senderIp,senderPort\0"
    "msearchReceived(QByteArray,QHostAddress*,quint16)\0"
    "datagramReceived()\0"
};

void Brisa::BrisaSSDPServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BrisaSSDPServer *_t = static_cast<BrisaSSDPServer *>(_o);
        switch (_id) {
        case 0: _t->msearchRequestReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 1: { bool _r = _t->isRunning();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: _t->start(); break;
        case 3: _t->stop(); break;
        case 4: _t->doNotify((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 5: _t->doByeBye((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->respondMSearch((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< const QString(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7])),(*reinterpret_cast< const QString(*)>(_a[8]))); break;
        case 7: _t->msearchReceived((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< QHostAddress*(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 8: _t->datagramReceived(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Brisa::BrisaSSDPServer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Brisa::BrisaSSDPServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Brisa__BrisaSSDPServer,
      qt_meta_data_Brisa__BrisaSSDPServer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Brisa::BrisaSSDPServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Brisa::BrisaSSDPServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Brisa::BrisaSSDPServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Brisa__BrisaSSDPServer))
        return static_cast<void*>(const_cast< BrisaSSDPServer*>(this));
    return QObject::qt_metacast(_clname);
}

int Brisa::BrisaSSDPServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Brisa::BrisaSSDPServer::msearchRequestReceived(const QString & _t1, const QString & _t2, quint16 _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
