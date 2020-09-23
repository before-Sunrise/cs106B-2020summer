/****************************************************************************
** Meta object code from reading C++ file 'gdownloader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../assign6-starter/lib/StanfordCPPLib/graphics/gdownloader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gdownloader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GDownloader_t {
    QByteArrayData data[13];
    char stringdata0[177];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GDownloader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GDownloader_t qt_meta_stringdata_GDownloader = {
    {
QT_MOC_LITERAL(0, 0, 11), // "GDownloader"
QT_MOC_LITERAL(1, 12, 10), // "downloaded"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 16), // "downloadInternal"
QT_MOC_LITERAL(4, 41, 17), // "fileDownloadError"
QT_MOC_LITERAL(5, 59, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(6, 87, 18), // "saveDownloadedData"
QT_MOC_LITERAL(7, 106, 11), // "std::string"
QT_MOC_LITERAL(8, 118, 6), // "member"
QT_MOC_LITERAL(9, 125, 8), // "filename"
QT_MOC_LITERAL(10, 134, 9), // "sslErrors"
QT_MOC_LITERAL(11, 144, 16), // "QList<QSslError>"
QT_MOC_LITERAL(12, 161, 15) // "waitForDownload"

    },
    "GDownloader\0downloaded\0\0downloadInternal\0"
    "fileDownloadError\0QNetworkReply::NetworkError\0"
    "saveDownloadedData\0std::string\0member\0"
    "filename\0sslErrors\0QList<QSslError>\0"
    "waitForDownload"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GDownloader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x08 /* Private */,
       4,    1,   51,    2, 0x08 /* Private */,
       6,    2,   54,    2, 0x08 /* Private */,
       6,    1,   59,    2, 0x28 /* Private | MethodCloned */,
      10,    1,   62,    2, 0x08 /* Private */,
      12,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 7,    8,    9,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void,

       0        // eod
};

void GDownloader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GDownloader *_t = static_cast<GDownloader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->downloaded(); break;
        case 1: _t->downloadInternal(); break;
        case 2: _t->fileDownloadError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 3: _t->saveDownloadedData((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const std::string(*)>(_a[2]))); break;
        case 4: _t->saveDownloadedData((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 5: _t->sslErrors((*reinterpret_cast< QList<QSslError>(*)>(_a[1]))); break;
        case 6: _t->waitForDownload(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (GDownloader::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GDownloader::downloaded)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject GDownloader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GDownloader.data,
      qt_meta_data_GDownloader,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GDownloader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GDownloader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GDownloader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GDownloader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void GDownloader::downloaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
