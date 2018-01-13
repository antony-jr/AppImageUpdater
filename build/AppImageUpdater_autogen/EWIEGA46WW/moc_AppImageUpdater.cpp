/****************************************************************************
** Meta object code from reading C++ file 'AppImageUpdater.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../AppImageUpdater.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppImageUpdater.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AppImageUpdater_t {
    QByteArrayData data[11];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AppImageUpdater_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AppImageUpdater_t qt_meta_stringdata_AppImageUpdater = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AppImageUpdater"
QT_MOC_LITERAL(1, 16, 22), // "safeToCloseApplication"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 15), // "checkForUpdates"
QT_MOC_LITERAL(4, 56, 8), // "progress"
QT_MOC_LITERAL(5, 65, 14), // "updateFinished"
QT_MOC_LITERAL(6, 80, 18), // "noUpdatesAvailable"
QT_MOC_LITERAL(7, 99, 16), // "updatesAvailable"
QT_MOC_LITERAL(8, 116, 11), // "handleError"
QT_MOC_LITERAL(9, 128, 6), // "cancel"
QT_MOC_LITERAL(10, 135, 4) // "exit"

    },
    "AppImageUpdater\0safeToCloseApplication\0"
    "\0checkForUpdates\0progress\0updateFinished\0"
    "noUpdatesAvailable\0updatesAvailable\0"
    "handleError\0cancel\0exit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AppImageUpdater[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x0a /* Public */,
       3,    1,   66,    2, 0x0a /* Public */,
       4,    5,   69,    2, 0x08 /* Private */,
       5,    2,   80,    2, 0x08 /* Private */,
       6,    2,   85,    2, 0x08 /* Private */,
       7,    2,   90,    2, 0x08 /* Private */,
       8,    2,   95,    2, 0x08 /* Private */,
       9,    0,  100,    2, 0x08 /* Private */,
      10,    0,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Float, QMetaType::LongLong, QMetaType::LongLong, QMetaType::Double, QMetaType::QString,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AppImageUpdater::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AppImageUpdater *_t = static_cast<AppImageUpdater *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->safeToCloseApplication(); break;
        case 1: _t->checkForUpdates(); break;
        case 2: _t->checkForUpdates((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->progress((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 4: _t->updateFinished((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->noUpdatesAvailable((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->updatesAvailable((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->handleError((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 8: _t->cancel(); break;
        case 9: _t->exit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AppImageUpdater::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AppImageUpdater::safeToCloseApplication)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AppImageUpdater::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_AppImageUpdater.data,
      qt_meta_data_AppImageUpdater,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AppImageUpdater::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppImageUpdater::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AppImageUpdater.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int AppImageUpdater::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AppImageUpdater::safeToCloseApplication()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
