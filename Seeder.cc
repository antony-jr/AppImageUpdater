#include <QMetaMethod>
#include <QMetaObject>

#include "Seeder.hpp"
#include "Seeder_p.hpp"

static QMetaMethod getMethod(QObject *object, const char *function) {
    auto metaObject = object->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}

Seeder::Seeder(QObject *parent)
    : QObject(parent) {
    m_Private = QSharedPointer<SeederPrivate>(new SeederPrivate);
    auto obj = m_Private.data();

    connect(obj, &SeederPrivate::errorSeeding,
            this, &Seeder::errorSeeding,
            Qt::DirectConnection);

    connect(obj, &SeederPrivate::queuedSeeding,
            this, &Seeder::queuedSeeding,
            Qt::DirectConnection);

    connect(obj, &SeederPrivate::startedSeeding,
            this, &Seeder::startedSeeding,
            Qt::DirectConnection);

    connect(obj, &SeederPrivate::removingSeeding,
            this, &Seeder::removingSeeding,
            Qt::DirectConnection);

    connect(obj, &SeederPrivate::stoppedSeeding,
            this, &Seeder::stoppedSeeding,
            Qt::DirectConnection);

    connect(obj, &SeederPrivate::torrentStatus,
            this, &Seeder::torrentStatus,
            Qt::DirectConnection);
}

void Seeder::updateProxy() {
    getMethod(m_Private.data(), "updateProxy(void)")
    .invoke(m_Private.data(),
            Qt::QueuedConnection);
}

void Seeder::startSeeding(QString hash, QString path, QUrl torrentFileUrl) {
    getMethod(m_Private.data(), "startSeeding(QString, QString, QUrl)")
    .invoke(m_Private.data(),
            Qt::QueuedConnection,
            Q_ARG(QString, hash),
	    Q_ARG(QString, path),
	    Q_ARG(QUrl, torrentFileUrl));

}

void Seeder::stopSeeding(QString hash) {
    getMethod(m_Private.data(), "stopSeeding(QString)")
    .invoke(m_Private.data(),
            Qt::QueuedConnection,
	    Q_ARG(QString, hash));
}
