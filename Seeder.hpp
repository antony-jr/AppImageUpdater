#ifndef SEEDER_HPP_INCLUDED
#define SEEDER_HPP_INCLUDED
#include <QObject>
#include <QJsonObject>
#include <QSharedPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class SeederPrivate;

class Seeder : public QObject {
    Q_OBJECT
    QSharedPointer<SeederPrivate> m_Private;
  public:
    Seeder(QObject *parent = nullptr);
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
    void torrentStatus(QString hash, int num_seeds, int num_peers); 
};
#endif // SEEDER_HPP_INCLUDED
