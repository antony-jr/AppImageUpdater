#include <QMetaMethod>
#include <QMetaObject>

#include "SeedManager.hpp"
#include "SeedManager_p.hpp"

static QMetaMethod getMethod(QObject *object, const char *function) {
    auto metaObject = object->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}

SeedManager::SeedManager(QObject *parent)
    : QObject(parent) {
    m_Private = QSharedPointer<SeedManagerPrivate>(new SeedManagerPrivate);
    auto obj = m_Private.data();

    connect(obj, &SeedManagerPrivate::errorSeeding,
            this, &SeedManager::errorSeeding,
            Qt::DirectConnection);

    connect(obj, &SeedManagerPrivate::queuedSeeding,
            this, &SeedManager::queuedSeeding,
            Qt::DirectConnection);

    connect(obj, &SeedManagerPrivate::startedSeeding,
            this, &SeedManager::startedSeeding,
            Qt::DirectConnection);

    connect(obj, &SeedManagerPrivate::removingSeeding,
            this, &SeedManager::removingSeeding,
            Qt::DirectConnection);

    connect(obj, &SeedManagerPrivate::stoppedSeeding,
            this, &SeedManager::stoppedSeeding,
            Qt::DirectConnection);

    connect(obj, &SeedManagerPrivate::torrentStatus,
            this, &SeedManager::torrentStatus,
            Qt::DirectConnection);
}

void SeedManager::updateProxy() {
    getMethod(m_Private.data(), "updateProxy(void)")
    .invoke(m_Private.data(),
            Qt::QueuedConnection);
}

void SeedManager::startSeeding(QString hash, QString path, QUrl torrentFileUrl) {
    getMethod(m_Private.data(), "startSeeding(QString, QString, QUrl)")
    .invoke(m_Private.data(),
            Qt::QueuedConnection,
            Q_ARG(QString, hash),
	    Q_ARG(QString, path),
	    Q_ARG(QUrl, torrentFileUrl));

}

void SeedManager::stopSeeding(QString hash) {
    getMethod(m_Private.data(), "stopSeeding(QString)")
    .invoke(m_Private.data(),
            Qt::QueuedConnection,
	    Q_ARG(QString, hash));
}
