/****************************************************************************
** Meta object code from reading C++ file 'mediarendererdeviceservice.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/device/av_device/mediarendererdeviceservice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mediarendererdeviceservice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MediaRendererDeviceService[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      68,   32,   28,   27, 0x02,
     177,  128,   28,   27, 0x02,
     298,  279,  261,   27, 0x02,
     345,  279,  261,   27, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_MediaRendererDeviceService[] = {
    "MediaRendererDeviceService\0\0int\0"
    "inArguments,action,errorDescription\0"
    "preAction(BrisaInArgument*const,BrisaAction*const,QString&)\0"
    "inArguments,outArguments,action,errorDescription\0"
    "postAction(BrisaInArgument*const,BrisaOutArgument*const,BrisaAction*co"
    "nst,QString&)\0"
    "BrisaOutArgument*\0inArguments,action\0"
    "offon(BrisaInArgument*const,BrisaAction*const)\0"
    "setchanel(BrisaInArgument*const,BrisaAction*const)\0"
};

void MediaRendererDeviceService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MediaRendererDeviceService *_t = static_cast<MediaRendererDeviceService *>(_o);
        switch (_id) {
        case 0: { int _r = _t->preAction((*reinterpret_cast< BrisaInArgument*const(*)>(_a[1])),(*reinterpret_cast< BrisaAction*const(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->postAction((*reinterpret_cast< BrisaInArgument*const(*)>(_a[1])),(*reinterpret_cast< BrisaOutArgument*const(*)>(_a[2])),(*reinterpret_cast< BrisaAction*const(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { BrisaOutArgument* _r = _t->offon((*reinterpret_cast< BrisaInArgument*const(*)>(_a[1])),(*reinterpret_cast< BrisaAction*const(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< BrisaOutArgument**>(_a[0]) = _r; }  break;
        case 3: { BrisaOutArgument* _r = _t->setchanel((*reinterpret_cast< BrisaInArgument*const(*)>(_a[1])),(*reinterpret_cast< BrisaAction*const(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< BrisaOutArgument**>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MediaRendererDeviceService::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MediaRendererDeviceService::staticMetaObject = {
    { &BrisaService::staticMetaObject, qt_meta_stringdata_MediaRendererDeviceService,
      qt_meta_data_MediaRendererDeviceService, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MediaRendererDeviceService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MediaRendererDeviceService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MediaRendererDeviceService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MediaRendererDeviceService))
        return static_cast<void*>(const_cast< MediaRendererDeviceService*>(this));
    return BrisaService::qt_metacast(_clname);
}

int MediaRendererDeviceService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BrisaService::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
