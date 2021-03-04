#include <QMetaMethod>
#include <QMetaObject>

#include "Executer_p.hpp"
#include "Executer.hpp"

static QMetaMethod getMethod(QObject *object, const char *function) {
    auto metaObject = object->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}


Executer::Executer(QObject *parent)
	: QObject(parent) {
	m_Private = new ExecuterPrivate;
	m_Thread = new QThread;
	m_Thread->start();

        m_Private->moveToThread(m_Thread);

	connect(m_Private, &ExecuterPrivate::loading,
		 this, &Executer::loading,
	 	 Qt::DirectConnection);

	connect(m_Private, &ExecuterPrivate::terminalApp,
		 this, &Executer::terminalApp,
	 	 Qt::DirectConnection);
	
	connect(m_Private, &ExecuterPrivate::finished,
		 this, &Executer::finished,
	 	 Qt::DirectConnection);
}	
Executer::~Executer() {
     m_Private->disconnect();
     m_Private->deleteLater();
     
     m_Thread->quit();
     m_Thread->wait();
     m_Thread->deleteLater();
}

void Executer::exec(const QString &hash, const QString &path) {
	getMethod(m_Private, "exec(const QString&, const QString&)")
    	.invoke(m_Private,
            	Qt::QueuedConnection,
		Q_ARG(QString, hash),
		Q_ARG(QString, path));

}
