#ifndef EXECUTER_PRIVATE_HPP_INCLUDED
#define EXECUTER_PRIVATE_HPP_INCLUDED
#include <QObject>
#include <QString> 
#include <QQueue>
#include <QPair>
#include <QTimer>
class ExecuterPrivate : public QObject {
	Q_OBJECT
	QPair<QString,QString> m_CurrentExec;
	QQueue<QPair<QString, QString>> m_Buffer;
public:
	ExecuterPrivate(QObject *parent = nullptr);
	~ExecuterPrivate();
public Q_SLOTS:
	void exec(const QString&, const QString&);
	void openDirectory(const QString&);
private Q_SLOTS:
	void finishExec();
private:
	void execNext();
Q_SIGNALS:
	void loading(QString hash);
	void terminalApp(QString hash);
	void finished(QString hash);
};

#endif // EXECUTER_PRIVATE_HPP_INCLUDED
