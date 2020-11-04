#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QStringList>
#include <QCoreApplication>
#include <QFile>
#include <DropItemParser.hpp>
#include <QDebug>

#include <appimage/core/AppImage.h>
#include <appimage/utils/ResourcesExtractor.h>
#include <sstream>

std::vector<std::string> split_string(const std::string& str,
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
static QPair<QString,QByteArray*> getAppImageNameAndIcon(const QString &appimagePath) {
	QPair <QString, QByteArray*> r;
	r.first = QString();
	r.second = nullptr;
	
	appimage::core::AppImage *appimage;
        appimage::utils::ResourcesExtractor *res;
	(appimagePath.toStdString());
	try {
		appimage = new appimage::core::AppImage(appimagePath.toStdString());
	}catch(...) {
		return r;
	}

	try {
		res = new appimage::utils::ResourcesExtractor(*appimage);
	}catch(...) {
		if(appimage) {
			delete appimage;
		}
		return r;
	}

	if(!res) {
		if(appimage) {
			delete appimage;
			return r;
		}
	}

	auto desktopFile = res->extract(res->getDesktopEntryPath());
	auto data = desktopFile.data();

	std::stringstream ss(data);
	std::string dest;
	while(std::getline(ss, dest, '\n')){
		if(dest == "[Desktop Entry]") {
			continue;
		}

		auto entry = split_string(dest, "=");
		if(entry.size() != 2) {
			continue;
		}

		if(entry[0] == "Name") {
			r.first = QString::fromStdString(entry[1]);
			if(r.second) {
				break;
			}
		}
		if(entry[0] == "Icon") {
			auto paths = res->getIconFilePaths(entry[1]);
			if(paths.size()) {
				auto iconData = res->extract(paths[paths.size()-1]);
				r.second = new QByteArray(iconData.data(), iconData.size());
			}
			if(!r.first.isEmpty()) {
				break;
			}
		}
		QCoreApplication::processEvents();
	}

	delete res;
	delete appimage;
	return r;
}

static QByteArray readLine(QFile *IO) {
    QByteArray ret;
    char c = 0;
    while(IO->getChar(&c) && c != '\n') {
        ret.append(c);
    }
    return ret;
}

static QByteArray getExecPathFromDesktopFile(QFile *file) {
    QByteArray line;
    qint64 prevPos = file->pos();
    file->seek(0);
    while(!(line = readLine(file)).isEmpty()) {
        if(line.contains("Exec")) {
            for(auto i = 0; i < line.size() ; ++i) {
                if(line[i] == '=') {
                    line = line.mid(i+1);
                    break;
                }
            }
            break;
        }
    }
    file->seek(prevPos);
    return line;
}


DropItemParser::DropItemParser(QObject *parent) 
	: QObject(parent) { }

DropItemParser::~DropItemParser() { }



/// Public Qt Slots
void DropItemParser::clearBuffer() {
	m_Buffer.clear();
}

void DropItemParser::appendToBuffer(const QString &path) {
	m_Buffer.append(path);
}

void DropItemParser::start() {
	if(m_Buffer.isEmpty()) {
		return;
	}
	emit loading();
	int enqueued = 0;
	for(auto i = 0; i < m_Buffer.size(); ++i) {	
	QFileInfo info(QUrl(m_Buffer.at(i)).toLocalFile());
	//// Let's check if it's a file or a directory
	if(info.isDir()) {
		//// If it's a directory then add AppImages recursively.
		QDirIterator dirIt(info.absoluteFilePath(),
                               QStringList() << "*.AppImage",
                               QDir::Files,
                               QDirIterator::Subdirectories);
            	while (dirIt.hasNext()) {
                	auto file = dirIt.next();

			/// Check if the file is actually an AppImage.
			auto appimageInformation = getAppImageNameAndIcon(file);
			if(appimageInformation.first.isEmpty() || 
			   appimageInformation.second == nullptr) {
				continue;
			}
			++enqueued;
			emit enqueue(file, appimageInformation.first, appimageInformation.second);
			QCoreApplication::processEvents();
            	}
		continue;
	}

	/// It is a file then try loading it as AppImage.
	auto appimageInformation = getAppImageNameAndIcon(info.absoluteFilePath());
	if(appimageInformation.first.isEmpty() || 
	   appimageInformation.second == nullptr) {
		/// Most probably not an AppImage. Let's see if it's Desktop file.
		
		QFile file(info.absoluteFilePath());
		if(!file.open(QIODevice::ReadOnly)) {
			continue;
		}
		auto signature = file.read(15);
		if(signature != "[Desktop Entry]") {
			continue;
		}
          	auto path = QString(getExecPathFromDesktopFile(&file));
            	if(path.isEmpty()) {
			continue;
		}

		QFileInfo pathInfo(path);

		//// The Exec field may be a command.
		if(!pathInfo.isFile() || !pathInfo.exists()) {
			continue;
		}

		//// If it's just relative then just fail.
		//// We can test the working dir entry in the 
		//// Desktop file but for now let's just do this.
		if(QFileInfo(path).isRelative()) {
			continue;
		}
		
		file.close();

		appimageInformation = getAppImageNameAndIcon(path);
		if(appimageInformation.first.isEmpty() || 
	   	   appimageInformation.second == nullptr) {
			continue;
		}
		++enqueued;
		emit enqueue(path, appimageInformation.first, appimageInformation.second);	
	}
	QCoreApplication::processEvents();
	}

	if(enqueued == 0){ 
		emit failed();
		return;
	}
	emit finished();
}
