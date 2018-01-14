/****************************************************************************
** Meta object code from reading C++ file 'AIUpdaterBridge.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../AppImageUpdaterBridge/AIUpdaterBridge.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AIUpdaterBridge.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AIUpdaterBridge_t {
    QByteArrayData data[29];
    char stringdata0[443];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AIUpdaterBridge_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AIUpdaterBridge_t qt_meta_stringdata_AIUpdaterBridge = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AIUpdaterBridge"
QT_MOC_LITERAL(1, 16, 7), // "stopped"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 16), // "updatesAvailable"
QT_MOC_LITERAL(4, 42, 18), // "noUpdatesAvailable"
QT_MOC_LITERAL(5, 61, 14), // "updateFinished"
QT_MOC_LITERAL(6, 76, 8), // "progress"
QT_MOC_LITERAL(7, 85, 12), // "bytesRecived"
QT_MOC_LITERAL(8, 98, 10), // "bytesTotal"
QT_MOC_LITERAL(9, 109, 5), // "speed"
QT_MOC_LITERAL(10, 115, 4), // "unit"
QT_MOC_LITERAL(11, 120, 5), // "error"
QT_MOC_LITERAL(12, 126, 9), // "isRunning"
QT_MOC_LITERAL(13, 136, 13), // "startUpdating"
QT_MOC_LITERAL(14, 150, 12), // "stopUpdating"
QT_MOC_LITERAL(15, 163, 5), // "clear"
QT_MOC_LITERAL(16, 169, 8), // "doUpdate"
QT_MOC_LITERAL(17, 178, 31), // "handleAppImageUpdateInformation"
QT_MOC_LITERAL(18, 210, 25), // "handleAppImageUpdateError"
QT_MOC_LITERAL(19, 236, 17), // "getGitHubReleases"
QT_MOC_LITERAL(20, 254, 23), // "getBintrayLatestPackage"
QT_MOC_LITERAL(21, 278, 26), // "handleBintrayLatestPackage"
QT_MOC_LITERAL(22, 305, 20), // "handleGitHubReleases"
QT_MOC_LITERAL(23, 326, 17), // "handleZsyncHeader"
QT_MOC_LITERAL(24, 344, 14), // "constructZsync"
QT_MOC_LITERAL(25, 359, 19), // "resolveRedirections"
QT_MOC_LITERAL(26, 379, 19), // "handleNetworkErrors"
QT_MOC_LITERAL(27, 399, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(28, 427, 15) // "checkForUpdates"

    },
    "AIUpdaterBridge\0stopped\0\0updatesAvailable\0"
    "noUpdatesAvailable\0updateFinished\0"
    "progress\0bytesRecived\0bytesTotal\0speed\0"
    "unit\0error\0isRunning\0startUpdating\0"
    "stopUpdating\0clear\0doUpdate\0"
    "handleAppImageUpdateInformation\0"
    "handleAppImageUpdateError\0getGitHubReleases\0"
    "getBintrayLatestPackage\0"
    "handleBintrayLatestPackage\0"
    "handleGitHubReleases\0handleZsyncHeader\0"
    "constructZsync\0resolveRedirections\0"
    "handleNetworkErrors\0QNetworkReply::NetworkError\0"
    "checkForUpdates"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AIUpdaterBridge[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  124,    2, 0x06 /* Public */,
       3,    2,  125,    2, 0x06 /* Public */,
       4,    2,  130,    2, 0x06 /* Public */,
       5,    2,  135,    2, 0x06 /* Public */,
       6,    5,  140,    2, 0x06 /* Public */,
      11,    2,  151,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  156,    2, 0x0a /* Public */,
      13,    0,  157,    2, 0x0a /* Public */,
      14,    0,  158,    2, 0x0a /* Public */,
      15,    0,  159,    2, 0x0a /* Public */,
      16,    0,  160,    2, 0x08 /* Private */,
      17,    2,  161,    2, 0x08 /* Private */,
      18,    2,  166,    2, 0x08 /* Private */,
      19,    1,  171,    2, 0x08 /* Private */,
      20,    1,  174,    2, 0x08 /* Private */,
      21,    1,  177,    2, 0x08 /* Private */,
      22,    1,  180,    2, 0x08 /* Private */,
      23,    2,  183,    2, 0x08 /* Private */,
      24,    0,  188,    2, 0x08 /* Private */,
      25,    2,  189,    2, 0x08 /* Private */,
      26,    1,  194,    2, 0x08 /* Private */,
      28,    0,  197,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Float, QMetaType::LongLong, QMetaType::LongLong, QMetaType::Double, QMetaType::QString,    2,    7,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    2,    2,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    2,    2,
    QMetaType::Void, QMetaType::QUrl,    2,
    QMetaType::Void, QMetaType::QUrl,    2,
    QMetaType::Void, QMetaType::QUrl,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    2,    2,
    QMetaType::Void, 0x80000000 | 27,    2,
    QMetaType::Void,

       0        // eod
};

void AIUpdaterBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AIUpdaterBridge *_t = static_cast<AIUpdaterBridge *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stopped(); break;
        case 1: _t->updatesAvailable((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->noUpdatesAvailable((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->updateFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->progress((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 5: _t->error((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 6: { bool _r = _t->isRunning();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->startUpdating(); break;
        case 8: _t->stopUpdating(); break;
        case 9: _t->clear(); break;
        case 10: _t->doUpdate(); break;
        case 11: _t->handleAppImageUpdateInformation((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 12: _t->handleAppImageUpdateError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 13: _t->getGitHubReleases((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 14: _t->getBintrayLatestPackage((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 15: _t->handleBintrayLatestPackage((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 16: _t->handleGitHubReleases((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->handleZsyncHeader((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 18: _t->constructZsync(); break;
        case 19: _t->resolveRedirections((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 20: _t->handleNetworkErrors((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 21: _t->checkForUpdates(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 20:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AIUpdaterBridge::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AIUpdaterBridge::stopped)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AIUpdaterBridge::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AIUpdaterBridge::updatesAvailable)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AIUpdaterBridge::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AIUpdaterBridge::noUpdatesAvailable)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AIUpdaterBridge::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AIUpdaterBridge::updateFinished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AIUpdaterBridge::*_t)(float , qint64 , qint64 , double , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AIUpdaterBridge::progress)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AIUpdaterBridge::*_t)(const QString & , short );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AIUpdaterBridge::error)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject AIUpdaterBridge::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AIUpdaterBridge.data,
      qt_meta_data_AIUpdaterBridge,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AIUpdaterBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AIUpdaterBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AIUpdaterBridge.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AIUpdaterBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void AIUpdaterBridge::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AIUpdaterBridge::updatesAvailable(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AIUpdaterBridge::noUpdatesAvailable(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AIUpdaterBridge::updateFinished(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AIUpdaterBridge::progress(float _t1, qint64 _t2, qint64 _t3, double _t4, const QString & _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AIUpdaterBridge::error(const QString & _t1, short _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
