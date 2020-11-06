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
		QString path;
		QString name;
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
	void queue(const QString&, const QString&, QVariant);
	void toggleNoConfirm();
	void continueCurrentUpdate();
	void cancelCurrentUpdate();
	void cancelAll();
private Q_SLOTS:
	void onFinishAction(QJsonObject, short);
	void onStartAction(short);
	void onCancelAction(short);
	void onErrorAction(short, short);
private:
	void updateNextAppImage();
Q_SIGNALS:
	void queuedCountChanged(int);
        void failedCountChanged(int);
	void completedCountChanged(int);

	void loading();
	void metaInfo(QJsonObject);
	void failed(QJsonObject);
	void finished(QJsonObject);
	void finishedAll();
};

#endif // UPDATER_PRIVATE_HPP_INCLUDED
