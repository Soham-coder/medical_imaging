/****************************************************************************
** Meta object code from reading C++ file 'labeldisp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imgProcess2D/labeldisp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'labeldisp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LabelDisp_t {
    QByteArrayData data[12];
    char stringdata[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_LabelDisp_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_LabelDisp_t qt_meta_stringdata_LabelDisp = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 17),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 7),
QT_MOC_LITERAL(4, 37, 13),
QT_MOC_LITERAL(5, 51, 5),
QT_MOC_LITERAL(6, 57, 8),
QT_MOC_LITERAL(7, 66, 14),
QT_MOC_LITERAL(8, 81, 8),
QT_MOC_LITERAL(9, 90, 9),
QT_MOC_LITERAL(10, 100, 7),
QT_MOC_LITERAL(11, 108, 3)
    },
    "LabelDisp\0sendMousePosition\0\0QPoint&\0"
    "sendPixelInfo\0QRgb&\0seedInfo\0"
    "getImgFileName\0fileName\0getPixmap\0"
    "QImage&\0img\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LabelDisp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x05,
       4,    1,   42,    2, 0x05,
       6,    2,   45,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       7,    1,   50,    2, 0x0a,
       9,    1,   53,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void LabelDisp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LabelDisp *_t = static_cast<LabelDisp *>(_o);
        switch (_id) {
        case 0: _t->sendMousePosition((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 1: _t->sendPixelInfo((*reinterpret_cast< QRgb(*)>(_a[1]))); break;
        case 2: _t->seedInfo((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< QRgb(*)>(_a[2]))); break;
        case 3: _t->getImgFileName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->getPixmap((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LabelDisp::*_t)(QPoint & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LabelDisp::sendMousePosition)) {
                *result = 0;
            }
        }
        {
            typedef void (LabelDisp::*_t)(QRgb & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LabelDisp::sendPixelInfo)) {
                *result = 1;
            }
        }
        {
            typedef void (LabelDisp::*_t)(QPoint & , QRgb & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LabelDisp::seedInfo)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject LabelDisp::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_LabelDisp.data,
      qt_meta_data_LabelDisp,  qt_static_metacall, 0, 0}
};


const QMetaObject *LabelDisp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LabelDisp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LabelDisp.stringdata))
        return static_cast<void*>(const_cast< LabelDisp*>(this));
    return QLabel::qt_metacast(_clname);
}

int LabelDisp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void LabelDisp::sendMousePosition(QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LabelDisp::sendPixelInfo(QRgb & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LabelDisp::seedInfo(QPoint & _t1, QRgb & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
