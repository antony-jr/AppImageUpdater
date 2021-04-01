#include <QCoreApplication>
#include <QFile>

#include "Helpers.hpp"

QString Helpers::fileNameFromPath(const QString &filePath) const {
	return QFileInfo(filePath).fileName();
}

bool Helpers::isMinimized() const {
  	auto arguments = QCoreApplication::arguments();
	if(arguments.size() != 1 && 
	   arguments.at(1).toLower() == QString::fromUtf8("--minimized")) {
		return true;
	}
	return false;
}

bool Helpers::removeFile(const QString &filePath) const {
	return QFile::remove(filePath);
}
