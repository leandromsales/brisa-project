/****************************************************************************
** Meta object code from reading C++ file 'webserversession.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/upnp/shared/webserver/webserversession.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'webserversession.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Brisa__WebserverSession[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,
      70,   54,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
     135,  117,   24,   24, 0x0a,
     164,  162,   24,   24, 0x2a,
     186,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Brisa__WebserverSession[] = {
    "Brisa::WebserverSession\0\0"
    "responsePosted(HttpResponse)\0"
    "request,session\0"
    "onEventReceived(HttpRequest,WebserverSession*)\0"
    "r,chunkedResponse\0respond(HttpResponse,bool)\0"
    "r\0respond(HttpResponse)\0onTimeout()\0"
};

void Brisa::WebserverSession::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WebserverSession *_t = static_cast<WebserverSession *>(_o);
        switch (_id) {
        case 0: _t->responsePosted((*reinterpret_cast< HttpResponse(*)>(_a[1]))); break;
        case 1: _t->onEventReceived((*reinterpret_cast< HttpRequest(*)>(_a[1])),(*reinterpret_cast< WebserverSession*(*)>(_a[2]))); break;
        case 2: _t->respond((*reinterpret_cast< HttpResponse(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->respond((*reinterpret_cast< HttpResponse(*)>(_a[1]))); break;
        case 4: _t->onTimeout(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Brisa::WebserverSession::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Brisa::WebserverSession::staticMetaObject = {
    { &HttpSession::staticMetaObject, qt_meta_stringdata_Brisa__WebserverSession,
      qt_meta_data_Brisa__WebserverSession, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Brisa::WebserverSession::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Brisa::WebserverSession::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Brisa::WebserverSession::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Brisa__WebserverSession))
        return static_cast<void*>(const_cast< WebserverSession*>(this));
    return HttpSession::qt_metacast(_clname);
}

int Brisa::WebserverSession::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = HttpSession::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Brisa::WebserverSession::responsePosted(HttpResponse _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Brisa::WebserverSession::onEventReceived(HttpRequest _t1, WebserverSession * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
