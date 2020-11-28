#include <QNetworkProxy>

#include "Updater_p.hpp"
#include "global.hpp"
#include "AppImageImageProvider.hpp"

#include <QCryptographicHash>

UpdaterPrivate::UpdaterPrivate(QObject *parent)
	: QObject(parent),
	  n_Queued(0),
	  n_Failed(0),
	  n_Completed(0),
	  b_NoConfirm(false),
	  b_Running(false)
{
	m_Updater = new QAppImageUpdate(/*single threaded=*/false, /*parent=*/this);

	connect(m_Updater, &QAppImageUpdate::finished, 
		 this, &UpdaterPrivate::onFinishAction,
		 Qt::QueuedConnection);
	connect(m_Updater, &QAppImageUpdate::error, 
		 this, &UpdaterPrivate::onErrorAction,
		 Qt::QueuedConnection);
	connect(m_Updater, &QAppImageUpdate::started, 
		 this, &UpdaterPrivate::onStartAction,
		 Qt::QueuedConnection);
	connect(m_Updater, &QAppImageUpdate::canceled, 
		 this, &UpdaterPrivate::onCancelAction,
		 Qt::QueuedConnection);
}

UpdaterPrivate::~UpdaterPrivate() {
	cancelAll();
}

void UpdaterPrivate::queue(const QString &path, const QString &name, QVariant icon) {
	if(path.isEmpty()) {
		return;
	}
	AppImage app;
	app.path = path;
	app.name = name;

	/// Provide the icon to the image provider.
	/// This is global variable, should be little careful with this.
	if(g_AppImageImageProvider) {
		QByteArray *array = new QByteArray(icon.toByteArray());	
		auto hash = QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Md4).toHex();
		app.image_id = QString::fromUtf8(hash);
		g_AppImageImageProvider->addImage(app.image_id, 
						  array); /// Will be using md4 hash of path as the id.	
	}

	m_AppImages.enqueue(app);

	++n_Queued;
	emit queuedCountChanged(n_Queued);
	
	QJsonObject r {
	   {"Hash", app.image_id },
	   {"AbsolutePath" , app.path},
	   {"Name", app.name},
	   {"ImageId", app.image_id}
	};
	emit queued(r);

	if(m_CurrentAppImage.isEmpty()) {
		updateNextAppImage();
	}
}

void UpdaterPrivate::toggleNoConfirm() {
	b_NoConfirm = !b_NoConfirm;
}

void UpdaterPrivate::continueCurrentUpdate() {
	m_Settings.sync();
	b_Running = true;
	bool useBt = m_Settings.value("V2.isDecentralizedUpdateEnabled").toBool();
	if(useBt) {
		m_Updater->start(QAppImageUpdate::Action::UpdateWithTorrent);
	}else{
		m_Updater->start();
	}
}

void UpdaterPrivate::cancelCurrentUpdate() {
	if(m_CurrentAppImage.isEmpty()) {
		return;
	}

	if(b_Running) {
		m_Updater->cancel();
	}else {
		updateNextAppImage();
	}
}

void UpdaterPrivate::cancelAll() {
	if(m_CurrentAppImage.isEmpty()) {
		return;
	}

	AppImage app;
	m_CurrentAppImage = app;
	m_AppImages.clear();
	m_Updater->cancel();
}

//// Private Qt Slots.
void UpdaterPrivate::onFinishAction(QJsonObject info, short action) {
	b_Running = false;
	qDebug() << "Finished Action";
	if(action == QAppImageUpdate::Action::Update ||
	   action == QAppImageUpdate::Action::UpdateWithTorrent) {
		++n_Completed;
		QJsonObject r {
			{"Updated" , true},
			{"NewAbsPath" , info["OldVersionPath"].toString() },
			{"OldAbsPath" , m_CurrentAppImage.path },
			{"Hash", m_CurrentAppImage.image_id},
			{"ImageId" , m_CurrentAppImage.image_id},
			{"Name" , m_CurrentAppImage.name }
		};
		emit finished(r);
		emit completedCountChanged(n_Completed);
		updateNextAppImage();
	}else if(action == QAppImageUpdate::Action::CheckForUpdate) {
		if(!info["UpdateAvailable"].toBool()) {
			++n_Completed;
			QJsonObject rt {
			{"Updated" , false},
			{"NewAbsPath" , m_CurrentAppImage.path },
			{"OldAbsPath" , m_CurrentAppImage.path },
			{"Hash", m_CurrentAppImage.image_id},
			{"ImageId" , m_CurrentAppImage.image_id},
			{"Name" , m_CurrentAppImage.name }
			};
	
			emit finished(rt);
			emit completedCountChanged(n_Completed);
			updateNextAppImage();
			return;
		}
		QJsonObject r {
			{"AbsolutePath", m_CurrentAppImage.path },
			{"ReleaseNotes", info["ReleaseNotes"].toString() },
			{"Hash", m_CurrentAppImage.image_id},
			{"ImageId" , m_CurrentAppImage.image_id},
			{"Name", m_CurrentAppImage.name }
		};
		emit metaInfo(r);
		
		if(b_NoConfirm) {
			m_Settings.sync();
			b_Running = true;
			bool useBt = m_Settings.value("V2.isDecentralizedUpdateEnabled").toBool();

			if(useBt){
				m_Updater->start(QAppImageUpdate::Action::UpdateWithTorrent);
			}else{
				m_Updater->start();
			}
		}
	}
}

void UpdaterPrivate::onStartAction(short action) {
	Q_UNUSED(action);
}

void UpdaterPrivate::onErrorAction(short code, short action) {
	Q_UNUSED(action);
	b_Running = false;
	QJsonObject r {
		{ "ErrorMsg" , QAppImageUpdate::errorCodeToDescriptionString(code) },
		{ "AbsolutePath" , m_CurrentAppImage.path} ,
		{ "Hash", m_CurrentAppImage.image_id}, 
		{ "ImageId" , m_CurrentAppImage.image_id},	
		{ "Name" , m_CurrentAppImage.name},
	};

	++n_Failed;
	emit failed(r);
	emit failedCountChanged(n_Failed);

	updateNextAppImage();
}

void UpdaterPrivate::onCancelAction(short action) {
	b_Running = false;
	Q_UNUSED(action);
	updateNextAppImage();
}

//// Private Methods
void UpdaterPrivate::updateNextAppImage() {
	emit loading();
	if(m_AppImages.isEmpty()) {
		n_Queued = 0;
		emit queuedCountChanged(0);
		emit finishedAll();
		return;
	}

	m_CurrentAppImage = m_AppImages.dequeue();
	--n_Queued;
	emit queuedCountChanged(n_Queued);

	m_Updater->clear();	
	m_Updater->setAppImage(m_CurrentAppImage.path);
	m_Settings.sync();
	bool useProxy = m_Settings.value("V2.isProxyEnabled").toBool();

	if(useProxy) {
		/// Get Proxy Settings
		m_Settings.sync();
		auto proxyHost = m_Settings.value("V2.ProxyHost").toString();
		auto proxyPort = m_Settings.value("V2.ProxyPort").toInt();
		auto proxyType = m_Settings.value("V2.ProxyType").toString();
		auto username = m_Settings.value("V2.ProxyUser").toString();
		auto passwd = m_Settings.value("V2.ProxyPass").toString();
		
		QNetworkProxy proxy;
		if(proxyType == "Socks 5") {
			proxy.setType(QNetworkProxy::Socks5Proxy);
		}else{
			proxy.setType(QNetworkProxy::HttpProxy);
		}
		proxy.setHostName(proxyHost);
		proxy.setPort(proxyPort);
		if(!username.isEmpty() && !passwd.isEmpty()) {
			proxy.setUser(username);
			proxy.setPassword(passwd);
		}
	}
	b_Running = true;
	m_Updater->start(QAppImageUpdate::Action::CheckForUpdate);
}
