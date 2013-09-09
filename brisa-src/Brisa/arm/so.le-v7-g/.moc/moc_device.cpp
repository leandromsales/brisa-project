/****************************************************************************
** Meta object code from reading C++ file 'device.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/controlpoint/device.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'device.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Device[] = {

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
       8,    7,    7,    7, 0x05,
      42,   40,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      88,    7,    7,    7, 0x0a,
     118,    7,    7,    7, 0x0a,
     158,   40,    7,    7, 0x0a,
     212,    7,    7,    7, 0x0a,
     240,   40,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Device[] = {
    "Device\0\0deviceDescriptionReady(Device*)\0"
    ",\0errorParsingDeviceDescription(Device*,quint8)\0"
    "replyFinished(QNetworkReply*)\0"
    "handleServiceDescriptionReady(Service*)\0"
    "handleErrorParsingServiceDescription(Service*,quint8)\0"
    "handleIconDownloaded(Icon*)\0"
    "handleErrorDownloadingIcon(Icon*,quint8)\0"
};

void Device::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Device *_t = static_cast<Device *>(_o);
        switch (_id) {
        case 0: _t->deviceDescriptionReady((*reinterpret_cast< Device*(*)>(_a[1]))); break;
        case 1: _t->errorParsingDeviceDescription((*reinterpret_cast< Device*(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 2: _t->replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 3: _t->handleServiceDescriptionReady((*reinterpret_cast< Service*(*)>(_a[1]))); break;
        case 4: _t->handleErrorParsingServiceDescription((*reinterpret_cast< Service*(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 5: _t->handleIconDownloaded((*reinterpret_cast< Icon*(*)>(_a[1]))); break;
        case 6: _t->handleErrorDownloadingIcon((*reinterpret_cast< Icon*(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Device::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Device::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Device,
      qt_meta_data_Device, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Device::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Device::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Device::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Device))
        return static_cast<void*>(const_cast< Device*>(this));
    return QObject::qt_metacast(_clname);
}

int Device::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Device::deviceDescriptionReady(Device * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Device::errorParsingDeviceDescription(Device * _t1, quint8 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
