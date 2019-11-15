/****************************************************************************
** Meta object code from reading C++ file 'LeptonThread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../LeptonThread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LeptonThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LeptonThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   33,   33,   33, 0x05,
      34,   33,   33,   33, 0x05,
      54,   33,   33,   33, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   33,   33,   33, 0x0a,
      93,   33,   33,   33, 0x0a,
     104,   33,   33,   33, 0x0a,
     114,   33,   33,   33, 0x0a,
     128,   33,   33,   33, 0x0a,
     141,   33,   33,   33, 0x0a,
     157,   33,   33,   33, 0x0a,
     171,   33,   33,   33, 0x0a,
     184,   33,   33,   33, 0x0a,
     195,   33,   33,   33, 0x0a,
     206,   33,   33,   33, 0x0a,
     228,   33,   33,   33, 0x0a,
     238,  255,   33,   33, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LeptonThread[] = {
    "LeptonThread\0updateText(QString)\0\0"
    "updateImage(QImage)\0updateRadiometry(QString)\0"
    "performFFC()\0snapshot()\0restart()\0"
    "disable_agc()\0enable_agc()\0serial_number()\0"
    "SysFpaTempK()\0FFC_Manual()\0RAD_Info()\0"
    "FFC_Auto()\0disable_TLinear_RAD()\0"
    "FPA_ROI()\0setColorMap(int)\0index\0"
};

void LeptonThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LeptonThread *_t = static_cast<LeptonThread *>(_o);
        switch (_id) {
        case 0: _t->updateText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->updateImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->updateRadiometry((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->performFFC(); break;
        case 4: _t->snapshot(); break;
        case 5: _t->restart(); break;
        case 6: _t->disable_agc(); break;
        case 7: _t->enable_agc(); break;
        case 8: _t->serial_number(); break;
        case 9: _t->SysFpaTempK(); break;
        case 10: _t->FFC_Manual(); break;
        case 11: _t->RAD_Info(); break;
        case 12: _t->FFC_Auto(); break;
        case 13: _t->disable_TLinear_RAD(); break;
        case 14: _t->FPA_ROI(); break;
        case 15: _t->setColorMap((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LeptonThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LeptonThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_LeptonThread,
      qt_meta_data_LeptonThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LeptonThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LeptonThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LeptonThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LeptonThread))
        return static_cast<void*>(const_cast< LeptonThread*>(this));
    return QThread::qt_metacast(_clname);
}

int LeptonThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void LeptonThread::updateText(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LeptonThread::updateImage(QImage _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LeptonThread::updateRadiometry(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
