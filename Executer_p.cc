#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QProcess>
#include <QTimer>
#include <QDir>
#include <QCoreApplication>
#include <QRegExp>
#include <QRegularExpression>
#include <QDesktopServices> 
#include <QUrl>

#include "Executer_p.hpp"

#include <appimage/core/AppImage.h>
#include <appimage/utils/ResourcesExtractor.h>
#include <sstream>
#include <vector>
#include <string>

static std::vector<std::string> split_string(const std::string& str,
                                      const std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    strings.push_back(str.substr(prev));
    return strings;
}

static bool isTerminalApplication(const QString &appimagePath) {
	appimage::core::AppImage *appimage;
        appimage::utils::ResourcesExtractor *res;
	try {
		appimage = new appimage::core::AppImage(appimagePath.toStdString());
	}catch(...) {
		qDebug() << "Cannot contstruct AppImage";
		return true; // If it's Terminal App then we will not execute it.
		/// It's better not execute something that is not an AppImage.
	}

	try {
		res = new appimage::utils::ResourcesExtractor(*appimage);
	}catch(...) {
		if(appimage) {
			delete appimage;
		}
		qDebug() << "Cannot Resource Extract";
		return true;
	}

	if(!res) {
		if(appimage) {
			delete appimage;
			return true;
		}
	}

	auto desktopFile = res->extract(res->getDesktopEntryPath());
	auto data = desktopFile.data();

	std::stringstream ss(data);
	std::string dest;

	bool terminalApp = true;
	while(std::getline(ss, dest, '\n')){
		if(dest == "[Desktop Entry]") {
			continue;
		}

		auto entry = split_string(dest, "=");
		if(entry.size() != 2) {
			continue;
		}

		if(entry[0] == "Terminal") {
			auto terminalValue = QString::fromStdString(entry[1]);
			if(terminalValue.toLower() == "false") {
				terminalApp = false;
			}
			break;
		}
		QCoreApplication::processEvents();
	}

	delete res;
	delete appimage;
	return terminalApp;
}



ExecuterPrivate::ExecuterPrivate(QObject *parent)
	: QObject(parent) {
}

ExecuterPrivate::~ExecuterPrivate() {

}

void ExecuterPrivate::openDirectory(const QString &path) {
	auto toOpen = QFileInfo(path).path();
	QDesktopServices::openUrl(QUrl(toOpen));
}

void ExecuterPrivate::exec(const QString &hash, const QString &path) {
	m_Buffer.enqueue(qMakePair<QString, QString>(hash, path));

	if(m_CurrentExec.first.isEmpty()) {
		execNext();
	}
}

void ExecuterPrivate::finishExec() {
	emit finished(m_CurrentExec.first);
	execNext();
}

void ExecuterPrivate::execNext() {
	if(m_Buffer.isEmpty()) {
		m_CurrentExec.first.clear();
		m_CurrentExec.second.clear();
		return;
	}

	emit loading(m_CurrentExec.first);
	m_CurrentExec = m_Buffer.dequeue();

	if(isTerminalApplication(m_CurrentExec.second)) {
		emit terminalApp(m_CurrentExec.first);
		execNext();
		return;
	}


	/// Make it executable if it's not.
 	QFileInfo info(m_CurrentExec.second);
        if(!info.isExecutable()) {
            {
                QFile file(m_CurrentExec.second);
                file.setPermissions(QFileDevice::ExeUser |
                                    QFileDevice::ExeOther|
                                    QFileDevice::ExeGroup|
                                    info.permissions());
            }
        }

	QProcess::startDetached(m_CurrentExec.second);
	QTimer::singleShot(2000, this, &ExecuterPrivate::finishExec);
}
