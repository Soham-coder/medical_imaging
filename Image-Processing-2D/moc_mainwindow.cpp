/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imgProcess2D/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[29];
    char stringdata[391];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 9),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 8),
QT_MOC_LITERAL(4, 31, 10),
QT_MOC_LITERAL(5, 42, 7),
QT_MOC_LITERAL(6, 50, 17),
QT_MOC_LITERAL(7, 68, 7),
QT_MOC_LITERAL(8, 76, 3),
QT_MOC_LITERAL(9, 80, 7),
QT_MOC_LITERAL(10, 88, 7),
QT_MOC_LITERAL(11, 96, 12),
QT_MOC_LITERAL(12, 109, 5),
QT_MOC_LITERAL(13, 115, 6),
QT_MOC_LITERAL(14, 122, 12),
QT_MOC_LITERAL(15, 135, 3),
QT_MOC_LITERAL(16, 139, 12),
QT_MOC_LITERAL(17, 152, 20),
QT_MOC_LITERAL(18, 173, 22),
QT_MOC_LITERAL(19, 196, 22),
QT_MOC_LITERAL(20, 219, 19),
QT_MOC_LITERAL(21, 239, 14),
QT_MOC_LITERAL(22, 254, 32),
QT_MOC_LITERAL(23, 287, 15),
QT_MOC_LITERAL(24, 303, 19),
QT_MOC_LITERAL(25, 323, 14),
QT_MOC_LITERAL(26, 338, 22),
QT_MOC_LITERAL(27, 361, 23),
QT_MOC_LITERAL(28, 385, 4)
    },
    "MainWindow\0loadImage\0\0fileName\0"
    "sendPixmap\0QImage&\0showMousePosition\0"
    "QPoint&\0pos\0getPos1\0getPos2\0getPixelInfo\0"
    "QRgb&\0pixVal\0getSeedInfo1\0val\0"
    "getSeedInfo2\0on_loadimage_clicked\0"
    "on_btnRefresh1_clicked\0on_btnRefresh2_clicked\0"
    "on_binarize_clicked\0on_dt8_clicked\0"
    "on_histogramequilization_clicked\0"
    "on_ccl4_clicked\0on_colormap_clicked\0"
    "on_FDT_clicked\0on_localmaxima_clicked\0"
    "on_spinBox_valueChanged\0arg1\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x05,
       4,    1,  112,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       6,    1,  115,    2, 0x0a,
       9,    1,  118,    2, 0x0a,
      10,    1,  121,    2, 0x0a,
      11,    1,  124,    2, 0x0a,
      14,    2,  127,    2, 0x0a,
      16,    2,  132,    2, 0x0a,
      17,    0,  137,    2, 0x08,
      18,    0,  138,    2, 0x08,
      19,    0,  139,    2, 0x08,
      20,    0,  140,    2, 0x08,
      21,    0,  141,    2, 0x08,
      22,    0,  142,    2, 0x08,
      23,    0,  143,    2, 0x08,
      24,    0,  144,    2, 0x08,
      25,    0,  145,    2, 0x08,
      26,    0,  146,    2, 0x08,
      27,    1,  147,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 12,    8,   15,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 12,    8,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   28,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->loadImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sendPixmap((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->showMousePosition((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 3: _t->getPos1((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 4: _t->getPos2((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 5: _t->getPixelInfo((*reinterpret_cast< QRgb(*)>(_a[1]))); break;
        case 6: _t->getSeedInfo1((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< QRgb(*)>(_a[2]))); break;
        case 7: _t->getSeedInfo2((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< QRgb(*)>(_a[2]))); break;
        case 8: _t->on_loadimage_clicked(); break;
        case 9: _t->on_btnRefresh1_clicked(); break;
        case 10: _t->on_btnRefresh2_clicked(); break;
        case 11: _t->on_binarize_clicked(); break;
        case 12: _t->on_dt8_clicked(); break;
        case 13: _t->on_histogramequilization_clicked(); break;
        case 14: _t->on_ccl4_clicked(); break;
        case 15: _t->on_colormap_clicked(); break;
        case 16: _t->on_FDT_clicked(); break;
        case 17: _t->on_localmaxima_clicked(); break;
        case 18: _t->on_spinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::loadImage)) {
                *result = 0;
            }
        }
        {
            typedef void (MainWindow::*_t)(QImage & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::sendPixmap)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::loadImage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::sendPixmap(QImage & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
