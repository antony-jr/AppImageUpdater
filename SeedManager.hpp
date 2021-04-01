#ifndef SEED_MANAGER_HPP_INCLUDED
#define SEED_MANAGER_HPP_INCLUDED
#include <QObject>
#include <QJsonObject>
#include <QSharedPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class SeedManagerPrivate;

class SeedManager : public QObject {
    Q_OBJECT
    QSharedPointer<SeedManagerPrivate> m_Private;
  public:
    SeedManager(QObject *parent = nullptr);
  public Q_SLOTS:
    void updateProxy();

    void startSeeding(QString, QString, QUrl);
    void stopSeeding(QString);
  Q_SIGNALS:
    void errorSeeding(QString hash);
    void queuedSeeding(QString hash);
    void startedSeeding(QString hash);
    void removingSeeding(QString hash);
    void stoppedSeeding(QString hash);
    void torrentStatus(QString hash, QString statusText);
};
#endif // SEED_MANAGER_HPP_INCLUDED
