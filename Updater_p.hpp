#ifndef UPDATER_PRIVATE_HPP_INCLUDED
#define UPDATER_PRIVATE_HPP_INCLUDED
#include <QObject>
#include <QQueue>
#include <QJsonObject>
#include <QAppImageUpdate>
#include <QSettings>
class UpdaterPrivate : public QObject {
	Q_OBJECT
	int n_Queued,
	    n_Failed,
	    n_Completed;
	bool b_NoConfirm;
	bool b_Running;
	struct AppImage {
		QString name;
		QString path;
		QString image_id;
		bool isEmpty() {
			return (path.isEmpty());
		}
	};
	AppImage m_CurrentAppImage;
	QQueue<AppImage> m_AppImages;
	QAppImageUpdate *m_Updater;
	QSettings m_Settings;
public:
	UpdaterPrivate(QObject *parent = nullptr);
	~UpdaterPrivate();
public Q_SLOTS:
	void retry(const QJsonObject&);
	void queue(const QString&, const QString&, QVariant);
	void toggleNoConfirm();
	void continueCurrentUpdate();
	void cancelCurrentUpdate();
	void cancelAll();
private Q_SLOTS:
	void onProgress(int, qint64, qint64, double, QString, short);
	void onFinishAction(QJsonObject, short);
	void onStartAction(short);
	void onCancelAction(short);
	void onErrorAction(short, short);
private:
	void updateNextAppImage();
Q_SIGNALS:
	void noConfirmState(bool);
	void queuedCountChanged(int);
        void failedCountChanged(int);
	void completedCountChanged(int);

	void progressText(QString, int);
	void loading();
	void metaInfo(QJsonObject);
	void queued(QJsonObject);
	void failed(QJsonObject);
	void started();
	void finished(QJsonObject);
	void retrySent(QString hash);
	void finishedAll();
};

#endif // UPDATER_PRIVATE_HPP_INCLUDED
