#ifndef DROP_PARSER_HPP_INCLUDED
#define DROP_PARSER_HPP_INCLUDED
#include <QObject>
#include <QStringList>
#include <QList>
#include <QString>
#include <QByteArray>
#include <QVariant>
class DropItemParser : public QObject {
	Q_OBJECT
	QStringList m_Buffer;
public:
	DropItemParser(QObject *parent = nullptr);
	~DropItemParser();
public Q_SLOTS:
	void clearBuffer();
	void appendToBuffer(const QString&);
	void start();
Q_SIGNALS:
	void enqueue(QString absolutePath, QString appName, QVariant icon);
	void loading();
	void failed();
	void finished();
};

#endif // DROP_PARSER_HPP_INCLUDED
