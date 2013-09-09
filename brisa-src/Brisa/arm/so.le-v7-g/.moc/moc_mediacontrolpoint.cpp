/****************************************************************************
** Meta object code from reading C++ file 'mediacontrolpoint.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/controlpoint/av_controlpoint/mediacontrolpoint.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mediacontrolpoint.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MediaControlPoint[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      78,   18,   18,   18, 0x05,
     141,   18,   18,   18, 0x05,
     173,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     207,   18,   18,   18, 0x08,
     237,   18,   18,   18, 0x08,
     275,  268,   18,   18, 0x08,
     307,  268,   18,   18, 0x08,
     358,  341,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MediaControlPoint[] = {
    "MediaControlPoint\0\0"
    "newMediaServerDevice(const MediaServerControlPointDevice*)\0"
    "newMediaRendererDevice(const MediaRendererControlPointDevice*)\0"
    "leaveMediaServerDevice(QString)\0"
    "leaveMediaRendererDevice(QString)\0"
    "handleNewSSDPMessage(QString)\0"
    "handleSSDPSetupEvents(QString)\0device\0"
    "handleMediaServerReady(Device*)\0"
    "handleMediaRendererReady(Device*)\0"
    "device,errorCode\0"
    "handleErrorParsingDeviceDescription(Device*,quint8)\0"
};

void MediaControlPoint::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MediaControlPoint *_t = static_cast<MediaControlPoint *>(_o);
        switch (_id) {
        case 0: _t->newMediaServerDevice((*reinterpret_cast< const MediaServerControlPointDevice*(*)>(_a[1]))); break;
        case 1: _t->newMediaRendererDevice((*reinterpret_cast< const MediaRendererControlPointDevice*(*)>(_a[1]))); break;
        case 2: _t->leaveMediaServerDevice((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->leaveMediaRendererDevice((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->handleNewSSDPMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->handleSSDPSetupEvents((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->handleMediaServerReady((*reinterpret_cast< Device*(*)>(_a[1]))); break;
        case 7: _t->handleMediaRendererReady((*reinterpret_cast< Device*(*)>(_a[1]))); break;
        case 8: _t->handleErrorParsingDeviceDescription((*reinterpret_cast< Device*(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MediaControlPoint::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MediaControlPoint::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MediaControlPoint,
      qt_meta_data_MediaControlPoint, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MediaControlPoint::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MediaControlPoint::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MediaControlPoint::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MediaControlPoint))
        return static_cast<void*>(const_cast< MediaControlPoint*>(this));
    return QObject::qt_metacast(_clname);
}

int MediaControlPoint::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void MediaControlPoint::newMediaServerDevice(const MediaServerControlPointDevice * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MediaControlPoint::newMediaRendererDevice(const MediaRendererControlPointDevice * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MediaControlPoint::leaveMediaServerDevice(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MediaControlPoint::leaveMediaRendererDevice(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
