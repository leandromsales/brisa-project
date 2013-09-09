/****************************************************************************
** Meta object code from reading C++ file 'httpsession.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/shared/webserver/http/httpsession.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'httpsession.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Brisa__HttpSession[] = {

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
      20,   19,   19,   19, 0x09,
      48,   19,   19,   19, 0x08,
      62,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Brisa__HttpSession[] = {
    "Brisa::HttpSession\0\0writeResponse(HttpResponse)\0"
    "onReadyRead()\0onConnectionDie()\0"
};

void Brisa::HttpSession::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HttpSession *_t = static_cast<HttpSession *>(_o);
        switch (_id) {
        case 0: _t->writeResponse((*reinterpret_cast< HttpResponse(*)>(_a[1]))); break;
        case 1: _t->onReadyRead(); break;
        case 2: _t->onConnectionDie(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Brisa::HttpSession::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Brisa::HttpSession::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Brisa__HttpSession,
      qt_meta_data_Brisa__HttpSession, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Brisa::HttpSession::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Brisa::HttpSession::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Brisa::HttpSession::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Brisa__HttpSession))
        return static_cast<void*>(const_cast< HttpSession*>(this));
    return QObject::qt_metacast(_clname);
}

int Brisa::HttpSession::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
