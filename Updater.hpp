#ifndef UPDATER_HPP_INCLUDED
#define UPDATER_HPP_INCLUDED
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <QThread>
#include <QVariant>
class UpdaterPrivate;
class Updater : public QObject {
	Q_OBJECT
	UpdaterPrivate *m_Private;
	QThread *m_Thread;
public:
	Updater(QObject *parent = nullptr);
	~Updater();
public Q_SLOTS:
	void queue(const QString&, const QString&, QVariant icon);
	void toggleNoConfirm();
	void continueCurrentUpdate();
	void cancelCurrentUpdate();
	void cancelAll();
Q_SIGNALS:
	void queuedCountChanged(int n);
        void failedCountChanged(int n);
	void completedCountChanged(int n);

	void loading();
	void metaInfo(QJsonObject info);
	void queued(QJsonObject info);
	void failed(QJsonObject info);
	void finished(QJsonObject info);
	void finishedAll();
};

#endif // UPDATER_HPP_INCLUDED
