/****************************************************************************
** Meta object code from reading C++ file 'QAIUpdateInformation.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../AppImageUpdaterBridge/QAIUpdateInformation/QAIUpdateInformation.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QAIUpdateInformation.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QAIUpdateInformation_t {
    QByteArrayData data[8];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QAIUpdateInformation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QAIUpdateInformation_t qt_meta_stringdata_QAIUpdateInformation = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QAIUpdateInformation"
QT_MOC_LITERAL(1, 21, 7), // "stopped"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 5), // "error"
QT_MOC_LITERAL(4, 36, 17), // "updateInformation"
QT_MOC_LITERAL(5, 54, 5), // "start"
QT_MOC_LITERAL(6, 60, 4), // "stop"
QT_MOC_LITERAL(7, 65, 22) // "parseUpdateInformation"

    },
    "QAIUpdateInformation\0stopped\0\0error\0"
    "updateInformation\0start\0stop\0"
    "parseUpdateInformation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QAIUpdateInformation[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    2,   45,    2, 0x06 /* Public */,
       4,    2,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   55,    2, 0x0a /* Public */,
       6,    0,   56,    2, 0x0a /* Public */,
       7,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Short,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QAIUpdateInformation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QAIUpdateInformation *_t = static_cast<QAIUpdateInformation *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stopped(); break;
        case 1: _t->error((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 2: _t->updateInformation((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 3: _t->start(); break;
        case 4: _t->stop(); break;
        case 5: _t->parseUpdateInformation(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (QAIUpdateInformation::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QAIUpdateInformation::stopped)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QAIUpdateInformation::*_t)(const QString & , short );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QAIUpdateInformation::error)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QAIUpdateInformation::*_t)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QAIUpdateInformation::updateInformation)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject QAIUpdateInformation::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QAIUpdateInformation.data,
      qt_meta_data_QAIUpdateInformation,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QAIUpdateInformation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QAIUpdateInformation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QAIUpdateInformation.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QAIUpdateInformation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QAIUpdateInformation::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QAIUpdateInformation::error(const QString & _t1, short _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QAIUpdateInformation::updateInformation(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
