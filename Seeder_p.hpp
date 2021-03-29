#ifndef SEEDER_PRIVATE_HPP_INCLUDED
#define SEEDER_PRIVATE_HPP_INCLUDED
#include <QObject>
#include <QQueue>
#include <QSettings>
#include <QMap>
#include <QScopedPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

#include <libtorrent/session.hpp>
#include <libtorrent/settings_pack.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/error_code.hpp>


class SeederPrivate : public QObject {
    Q_OBJECT
  public:
    SeederPrivate();
    ~SeederPrivate();
  public Q_SLOTS:
    void updateProxy();

    void startSeeding(QString, QString, QUrl);
    void stopSeeding(QString);

  private Q_SLOTS:
    void getNextTorrentMeta();
    void handleTorrentFileData(qint64, qint64);
    void handleTorrentFileError(QNetworkReply::NetworkError);
    void handleTorrentFileFinish();

    void torrentLoop();

  Q_SIGNALS:
    void errorSeeding(QString);
    void queuedSeeding(QString);
    void startedSeeding(QString);
    void removingSeeding(QString);
    void stoppedSeeding(QString);
    void torrentStatus(QString, QString);
  private:
    struct AppImageSeedInfo {
	    QString hash;
	    QString path;
	    QUrl torrentFileUrl;
	    bool empty = true;
    };

    QTimer m_Timer;
    AppImageSeedInfo m_CurrentTorrent;
    QQueue<AppImageSeedInfo> m_QueuedTorrents;
    QList<QString> m_ReqTorrentForRemoval;
    QMap<QString, lt::torrent_handle> m_Torrents;
    QScopedPointer<QNetworkAccessManager> m_Manager;
    QScopedPointer<QByteArray> m_TorrentMeta;
    QScopedPointer<lt::session> m_Session;
};
#endif // SEEDER_PRIVATE_HPP_INCLUDED
