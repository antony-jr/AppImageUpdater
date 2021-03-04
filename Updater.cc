#include <QMetaMethod>
#include <QMetaObject>

#include "Updater.hpp"
#include "Updater_p.hpp"

static QMetaMethod getMethod(QObject *object, const char *function) {
    auto metaObject = object->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}



Updater::Updater(QObject *parent) 
	: QObject(parent) {
	b_NoConfirm = false;	
	m_Private = new UpdaterPrivate;
	m_Thread = new QThread;
	m_Thread->start();

        m_Private->moveToThread(m_Thread); 

	connect(m_Private, &UpdaterPrivate::clearLog,
		 this, &Updater::clearLog);

	connect(m_Private, &UpdaterPrivate::appendLog,
		 this, &Updater::appendLog);

	connect(m_Private, &UpdaterPrivate::removedFromQueue,
		    this, &Updater::removedFromQueue);

	connect(m_Private, &UpdaterPrivate::queuedCountChanged,
		 this, &Updater::queuedCountChanged);

        connect(m_Private, &UpdaterPrivate::failedCountChanged,
		  this, &Updater::failedCountChanged);

	connect(m_Private, &UpdaterPrivate::completedCountChanged,
		   this, &Updater::completedCountChanged);

	connect(m_Private, &UpdaterPrivate::progressText,
		    this, &Updater::progressText);

	connect(m_Private, &UpdaterPrivate::loading,
		    this, &Updater::loading);

	connect(m_Private, &UpdaterPrivate::metaInfo,
		    this, &Updater::metaInfo);
	
	connect(m_Private, &UpdaterPrivate::queued,
		    this, &Updater::queued);

	connect(m_Private, &UpdaterPrivate::failed,
		    this, &Updater::failed);

	connect(m_Private, &UpdaterPrivate::started,
		    this, &Updater::started);

	connect(m_Private, &UpdaterPrivate::finished,
		    this, &Updater::finished);
	
	connect(m_Private, &UpdaterPrivate::canceled,
		    this, &Updater::canceled);
	

	connect(m_Private, &UpdaterPrivate::retrySent,
		    this, &Updater::retrySent);


	connect(m_Private, &UpdaterPrivate::finishedAll,
		    this, &Updater::finishedAll);

	connect(m_Private, &UpdaterPrivate::noConfirmState,
		    this,  &Updater::handleNoConfirmState); 
}

Updater::~Updater() {
     m_Private->disconnect();
     m_Private->cancelAll();
     m_Private->deleteLater();

     m_Thread->quit();
     m_Thread->wait();
     m_Thread->deleteLater();
}

bool Updater::isNoConfirm() const {
	return b_NoConfirm;
}

void Updater::handleNoConfirmState(bool value) {
	b_NoConfirm = value;
	emit isNoConfirmEnabledChanged();
}

void Updater::retry(const QJsonObject &json) {
	getMethod(m_Private, "retry(const QJsonObject&)")
    	.invoke(m_Private,
            	Qt::QueuedConnection,
		Q_ARG(QJsonObject, json));	
}

void Updater::removeFromQueue(const QString &hash) {
	getMethod(m_Private, "removeFromQueue(const QString&)")
    	.invoke(m_Private,
            	Qt::QueuedConnection,
		Q_ARG(QString, hash));
}

void Updater::queue(const QString &path, const QString &name, QVariant icon) {
	getMethod(m_Private, "queue(const QString&, const QString&, QVariant)")
    	.invoke(m_Private,
            	Qt::QueuedConnection,
		Q_ARG(QString, path),
		Q_ARG(QString, name),
            	Q_ARG(QVariant,icon));
}

void Updater::toggleNoConfirm() {
	getMethod(m_Private, "toggleNoConfirm()")
    	.invoke(m_Private,
            	Qt::QueuedConnection); 
}

void Updater::continueCurrentUpdate() {
	getMethod(m_Private, "continueCurrentUpdate()")
    	.invoke(m_Private,
            	Qt::QueuedConnection);
}

void Updater::cancelCurrentUpdate() {
	getMethod(m_Private, "cancelCurrentUpdate()")
    	.invoke(m_Private,
            	Qt::QueuedConnection);
}

void Updater::cancelAll() {
	getMethod(m_Private, "cancelAll()")
    	.invoke(m_Private,
            	Qt::QueuedConnection);
}
