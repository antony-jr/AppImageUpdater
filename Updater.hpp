#ifndef UPDATER_HPP_INCLUDED
#define UPDATER_HPP_INCLUDED
#include <QObject>
#include <QStringList>
class Updater : public QObject {
	Q_OBJECT
	int n_Queued,
	    n_Failed,
	    n_Completed;
	struct AppImage {
		QString path;
		QString name;
		QByteArray *icon = nullptr;
	};
	QVector<AppImage> m_AppImages;
public:
	Updater(QObject *parent = nullptr);
	~Updater();
public Q_SLOTS:
	void queue(const QString&, const QString&, QByteArray*);
	void start();
Q_SIGNALS:
	void queuedCountChanged(int);
        void failedCountChanged(int);
	void completedCountChanged(int);

	void loading();
	void finished();
};

#endif // UPDATER_HPP_INCLUDED
