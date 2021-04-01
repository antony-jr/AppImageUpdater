#ifndef EXECUTER_HPP_INCLUDED
#define EXECUTER_HPP_INCLUDED
#include <QObject>
#include <QString>
#include <QThread>
class ExecuterPrivate;
class Executer : public QObject {
	Q_OBJECT
	ExecuterPrivate *m_Private;
	QThread *m_Thread;

public:
	Executer(QObject *parent = nullptr);
	~Executer();
public Q_SLOTS:
	void exec(const QString&, const QString&);
	void openDirectory(const QString&);
Q_SIGNALS:
	void loading(QString hash);
	void terminalApp(QString hash);
	void finished(QString hash);
};

#endif // EXECUTER_HPP_INCLUDED
