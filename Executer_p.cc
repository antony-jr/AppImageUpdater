#include <QProcess>
#include <QTimer>
#include <QDir>
#include <QCoreApplication>
#include <QRegExp>
#include <QRegularExpression>

#include "Executer_p.hpp"
#include "Version.hpp"


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


static QString toNativePath(const QString &path)
{
    return QDir::toNativeSeparators(path);
}

static QString toUniformPath(const QString &path)
{
    return QDir::fromNativeSeparators(path);
}


static void openFolderSelect(const QString &absolutePath)
{
    QString path {toUniformPath(absolutePath)};
    const QFileInfo pathInfo {path};
    if (!pathInfo.exists(path))
    {
        return;
    }

    QProcess proc;
    proc.start("xdg-mime", {"query", "default", "inode/directory"});
    proc.waitForFinished();
    const QString output = proc.readLine().simplified();
    if ((output == "dolphin.desktop") || (output == "org.kde.dolphin.desktop"))
    {
        proc.startDetached("dolphin", {"--select", toNativePath(path)});
    }
    else if ((output == "nautilus.desktop") || (output == "org.gnome.Nautilus.desktop")
                 || (output == "nautilus-folder-handler.desktop"))
                 {
        if (pathInfo.isDir())
            path = path.left(path.lastIndexOf('/'));
        proc.start("nautilus", {"--version"});
        proc.waitForFinished();
        const QString nautilusVerStr = QString(proc.readLine()).remove(QRegularExpression("[^0-9.]"));
        using NautilusVersion = Utils::Version<int, 3>;
        if (NautilusVersion::tryParse(nautilusVerStr, {1, 0, 0}) > NautilusVersion {3, 28})
            proc.startDetached("nautilus", {toNativePath(path)});
        else
            proc.startDetached("nautilus", {"--no-desktop", toNativePath(path)});
    }
    else if (output == "nemo.desktop")
    {
        if (pathInfo.isDir())
            path = path.left(path.lastIndexOf('/'));
        proc.startDetached("nemo", {"--no-desktop", toNativePath(path)});
    }
    else if ((output == "konqueror.desktop") || (output == "kfmclient_dir.desktop"))
    {
        proc.startDetached("konqueror", {"--select", toNativePath(path)});
    }
    return;
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

void ExecuterPrivate::exec(const QString &hash, const QString &path) {
	m_Buffer.enqueue(qMakePair<QString, QString>(hash, path));

	if(m_CurrentExec.first.isEmpty()) {
		execNext();
	}
}

void ExecuterPrivate::openDirectory(const QString &path) {
	openFolderSelect(path);
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
	QProcess::startDetached(m_CurrentExec.second);
	QTimer::singleShot(2000, this, &ExecuterPrivate::finishExec);
}
