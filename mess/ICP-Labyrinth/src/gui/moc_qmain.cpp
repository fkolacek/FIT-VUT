/****************************************************************************
** Meta object code from reading C++ file 'qmain.h'
**
** Created: Sun May 10 17:58:30 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qmain.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qmain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QMain[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x05,
      30,   28,    6,    6, 0x05,
      54,    6,    6,    6, 0x25,

 // slots: signature, parameters, type, tag, flags
      74,    6,    6,    6, 0x0a,
      91,    6,    6,    6, 0x0a,
     111,    6,    6,    6, 0x0a,
     130,    6,    6,    6, 0x0a,
     149,    6,    6,    6, 0x0a,
     169,    6,    6,    6, 0x0a,
     190,    6,    6,    6, 0x0a,
     211,    6,    6,    6, 0x0a,
     240,    6,    6,    6, 0x0a,
     259,    6,    6,    6, 0x0a,
     273,    6,    6,    6, 0x0a,
     288,  285,    6,    6, 0x0a,
     312,  285,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QMain[] = {
    "QMain\0\0sendMessage(QString)\0,\0"
    "logMessage(QString,int)\0logMessage(QString)\0"
    "handleShowHelp()\0handleShowConnect()\0"
    "handleDisconnect()\0handleShowInvite()\0"
    "handleShowNewGame()\0handleShowLoadGame()\0"
    "handleShowSaveGame()\0handleWorkerMessage(QString)\0"
    "handleWorkerDied()\0debugToggle()\0"
    "timerTick()\0,,\0handleMove(int,int,int)\0"
    "handlePut(int,int,stoneAngle)\0"
};

const QMetaObject QMain::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QMain,
      qt_meta_data_QMain, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMain::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMain))
        return static_cast<void*>(const_cast< QMain*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: logMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: logMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: handleShowHelp(); break;
        case 4: handleShowConnect(); break;
        case 5: handleDisconnect(); break;
        case 6: handleShowInvite(); break;
        case 7: handleShowNewGame(); break;
        case 8: handleShowLoadGame(); break;
        case 9: handleShowSaveGame(); break;
        case 10: handleWorkerMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: handleWorkerDied(); break;
        case 12: debugToggle(); break;
        case 13: timerTick(); break;
        case 14: handleMove((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 15: handlePut((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< stoneAngle(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void QMain::sendMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QMain::logMessage(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
