/****************************************************************************
** Meta object code from reading C++ file 'icon.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/controlpoint/icon.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'icon.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Icon[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x05,
      30,   28,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
      65,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Icon[] = {
    "Icon\0\0iconDownloaded(Icon*)\0,\0"
    "errorDownloadingIcon(Icon*,quint8)\0"
    "replyFinished(QNetworkReply*)\0"
};

void Icon::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Icon *_t = static_cast<Icon *>(_o);
        switch (_id) {
        case 0: _t->iconDownloaded((*reinterpret_cast< Icon*(*)>(_a[1]))); break;
        case 1: _t->errorDownloadingIcon((*reinterpret_cast< Icon*(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 2: _t->replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Icon::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Icon::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Icon,
      qt_meta_data_Icon, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Icon::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Icon::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Icon::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Icon))
        return static_cast<void*>(const_cast< Icon*>(this));
    return QObject::qt_metacast(_clname);
}

int Icon::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Icon::iconDownloaded(Icon * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Icon::errorDownloadingIcon(Icon * _t1, quint8 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
