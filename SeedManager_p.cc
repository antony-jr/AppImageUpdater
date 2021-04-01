#include <QFileInfo>
#include <QNetworkProxy>
#include <QThread>
#include <QCoreApplication>
#include <QNetworkProxy>
#include <vector>
#include <iostream>

#include "SeedManager_p.hpp"

SeedManagerPrivate::SeedManagerPrivate()
    : QObject() {

    m_Manager.reset(new QNetworkAccessManager);
    lt::session_params p = lt::session_params();
    p.settings.set_int(lt::settings_pack::alert_mask,
                       lt::alert_category::status |
                       lt::alert_category::error |
                       lt::alert_category::storage);

    m_Session.reset(new lt::session(p));
    m_TorrentMeta.reset(new QByteArray);

    m_Timer.setSingleShot(false);
    m_Timer.setInterval(100); // 1ms?
    connect(&m_Timer, &QTimer::timeout,
            this, &SeedManagerPrivate::torrentLoop,
            Qt::QueuedConnection);

    updateProxy();

    m_Timer.setSingleShot(false);
    m_Timer.setInterval(100);
    m_Timer.start();
}

SeedManagerPrivate::~SeedManagerPrivate() {
	m_Session->abort();
}

void SeedManagerPrivate::updateProxy() {
    QNetworkProxy proxy;
    lt::settings_pack sp;
    QSettings settings;
    settings.sync();

    /// Get Proxy Settings
    auto useProxy = settings.value("V2.isProxyEnabled").toBool();
    auto phost = settings.value("V2.ProxyHost").toString();
    auto pport = settings.value("V2.ProxyPort").toInt();
    auto ptype = settings.value("V2.ProxyType").toString();
    auto puser = settings.value("V2.ProxyUser").toString();
    auto ppass = settings.value("V2.ProxyPass").toString();

    if(!useProxy) {
    if(!m_Manager.isNull()) {
    	    m_Manager->setProxy(proxy);
    }

    if(!m_Session.isNull()) {
	    m_Session->apply_settings(sp);
    }
	    return;
    }

    /// Check if proxy provided
    if(!phost.isEmpty() && pport != 0) {

    //// Set proxy for QNetworkAccessManager.
    proxy.setHostName(phost);
    proxy.setPort(pport);
 
    //// Set proxy for libtorrent.
    sp.set_str(lt::settings_pack::proxy_hostname,
			   phost.toStdString());
    sp.set_int(lt::settings_pack::proxy_port,
			   (int)pport);
    
    if(ptype == "Socks 5") {
    	proxy.setType(QNetworkProxy::Socks5Proxy);
    	sp.set_int(lt::settings_pack::proxy_type,
			   lt::settings_pack::socks5_pw);
	
    } else {
	proxy.setType(QNetworkProxy::HttpProxy);
    	sp.set_int(lt::settings_pack::proxy_type,
			   lt::settings_pack::http_pw);
	
    }
    proxy.setHostName(phost);
    proxy.setPort(pport);

    if(!puser.isEmpty() && !ppass.isEmpty()) {
	proxy.setUser(puser);
	proxy.setPassword(ppass);
	
	sp.set_str(lt::settings_pack::proxy_username,
			   puser.toStdString());
	sp.set_str(lt::settings_pack::proxy_password,
			   ppass.toStdString());
	
    }

    if(!m_Manager.isNull()) {
    	    m_Manager->setProxy(proxy);
    }

    if(!m_Session.isNull()) {
	    m_Session->apply_settings(sp);
    }
    }
}

void SeedManagerPrivate::startSeeding(QString hash, QString path, QUrl torrentFile) {
    if(m_Torrents.contains(hash)) {
	    return;
    }

    AppImageSeedInfo info;
    info.hash = hash;
    info.path = path;
    info.torrentFileUrl = torrentFile;
    info.empty = false;

    m_QueuedTorrents.enqueue(info);
    emit queuedSeeding(info.hash);

    if(m_CurrentTorrent.empty) {
	    getNextTorrentMeta();
    }
    return;
}

void SeedManagerPrivate::stopSeeding(QString hash) {
    if(!m_Torrents.contains(hash)) {
	    return;
    }

    m_ReqTorrentForRemoval.append(hash);
    emit removingSeeding(hash);
}

void SeedManagerPrivate::getNextTorrentMeta() {
    if(m_QueuedTorrents.isEmpty()) {
	    AppImageSeedInfo empty;
	    m_CurrentTorrent = empty; 
	    return;
    }

    m_CurrentTorrent = m_QueuedTorrents.dequeue();

    m_TorrentMeta->clear();
    updateProxy();

    QNetworkRequest request;
    request.setUrl(m_CurrentTorrent.torrentFileUrl);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    auto reply = m_Manager->get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(handleTorrentFileError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(handleTorrentFileData(qint64, qint64)));
    connect(reply, SIGNAL(finished()),
            this, SLOT(handleTorrentFileFinish()));
}

void SeedManagerPrivate::handleTorrentFileError(QNetworkReply::NetworkError code) {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    Q_UNUSED(code);
    if(!reply) {
        return;
    }

    reply->disconnect();
    reply->deleteLater();

    emit errorSeeding(m_CurrentTorrent.hash);

    getNextTorrentMeta();
}

void SeedManagerPrivate::handleTorrentFileData(qint64 br, qint64 bt) {
    Q_UNUSED(br);
    Q_UNUSED(bt);

    auto reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(!reply) {
        return;
    }

    if(reply->error() != QNetworkReply::NoError) {
        return;
    }

    if(reply->isReadable()) {
        m_TorrentMeta->append(reply->readAll());
    }
}

void SeedManagerPrivate::handleTorrentFileFinish() {
    auto reply = qobject_cast<QNetworkReply*>(QObject::sender());
    m_TorrentMeta->append(reply->readAll());

    reply->disconnect();
    reply->deleteLater();

    lt::add_torrent_params params;
    QString savePath = QFileInfo(m_CurrentTorrent.path).path() + "/";

    params.save_path = savePath.toStdString();
    auto ti = std::make_shared<lt::torrent_info>(m_TorrentMeta->constData(), (int)m_TorrentMeta->size());

    /// We know that MakeAppImageTorrent only packs a single file that is the
    /// the Target AppImage. So We just need to check if it is bundled correctly.
    if(ti->num_files() != 1) {
	emit errorSeeding(m_CurrentTorrent.hash);
	getNextTorrentMeta();
        return;
    }

    /// Since only 1 file is packaged in the torrent, we can
    /// assume that the file index for our Target AppImage is 0
    ti->rename_file(0,
                    QFileInfo(m_CurrentTorrent.path).fileName().toStdString());

    params.ti = ti;
    auto handle = m_Session->add_torrent(params);
    if(!handle.is_valid()) {
         emit errorSeeding(m_CurrentTorrent.hash);
	 getNextTorrentMeta();
	 return;
    }
    
    m_Torrents.insert(m_CurrentTorrent.hash, handle);
    emit startedSeeding(m_CurrentTorrent.hash);

    getNextTorrentMeta();
    return;
}

void SeedManagerPrivate::torrentLoop() {
    if(m_Session.isNull()) {
	    return;
    }

    if(m_Torrents.isEmpty()) {
	    return;
    }

    QList<QString> toRemove;
    QString torrentStatusTextFormat = QString::fromUtf8("Seeding at %1 KiB/s to %2 peer(s), uploaded %3 MiB in total.");

    for(auto key: m_Torrents.keys()) {
	    auto handle = m_Torrents[key];
	    auto status = handle.status();

	    emit torrentStatus(key,
	    torrentStatusTextFormat.arg(status.upload_rate/1024).arg(status.num_peers).arg(status.all_time_upload/1048576));
	    if(m_ReqTorrentForRemoval.contains(key)) {
		m_Session->remove_torrent(handle);
		toRemove.append(key);
		m_ReqTorrentForRemoval.removeAll(key);
		emit stoppedSeeding(key);
		continue;
	    }
	    if(status.state == lt::torrent_status::downloading) {
		    /// If it is not seeding but downloading then something is wrong.
		    emit errorSeeding(key);
		    m_Session->remove_torrent(handle);
		    toRemove.append(key);
	    }
	    QCoreApplication::processEvents();
    }

    /// Remove all defered torrent seeding
    for(auto key: toRemove) {
	    m_Torrents.remove(key);
    }
}
