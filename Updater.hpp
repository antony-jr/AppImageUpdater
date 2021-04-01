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
	Q_PROPERTY(bool isNoConfirmEnabled READ isNoConfirm NOTIFY isNoConfirmEnabledChanged)

	bool b_NoConfirm;
	UpdaterPrivate *m_Private;
	QThread *m_Thread;
public:
	Updater(QObject *parent = nullptr);
	~Updater();
	
	bool isNoConfirm() const;
private Q_SLOTS:
	void handleNoConfirmState(bool);
public Q_SLOTS:
	void removeFromQueue(const QString&);
	void queue(const QString&, const QString&, QVariant icon);
	void retry(const QJsonObject&);
	void toggleNoConfirm();
	void continueCurrentUpdate();
	void cancelCurrentUpdate();
	void cancelAll();
Q_SIGNALS:
	void isNoConfirmEnabledChanged();
	void removedFromQueue(QString hash);
	void queuedCountChanged(int n);
        void failedCountChanged(int n);
	void completedCountChanged(int n);

	void progressText(QString progressTextString, int progressValue);
	void torrentStatus(int seeders, int peers);
	void loading();
	void metaInfo(QJsonObject info);
	void queued(QJsonObject info);
	void failed(QJsonObject info);
	void started();
	void finished(QJsonObject info);
	void canceled(QString hash);
	void retrySent(QString hash);
	void finishedAll();
};

#endif // UPDATER_HPP_INCLUDED
