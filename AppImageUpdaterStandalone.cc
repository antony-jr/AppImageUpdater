#include <AppImageUpdaterStandalone.hpp>
#include <QFileInfo>
#include <QScreen>
#include <QMessageBox>
#include <QBuffer>

#include <appimage/core/AppImage.h>
#include <appimage/utils/ResourcesExtractor.h>
#include <sstream>

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

static QPair<QString,QByteArray> getAppImageNameAndIcon(const QString &appimagePath) {
	QPair <QString, QByteArray> r;
	r.first = QString();
	r.second = QByteArray();
	
	appimage::core::AppImage *appimage;
        appimage::utils::ResourcesExtractor *res;
	try {
		appimage = new appimage::core::AppImage(appimagePath.toStdString());
	}catch(...) {
		qDebug() << "Cannot contstruct AppImage";
		return r;
	}

	try {
		res = new appimage::utils::ResourcesExtractor(*appimage);
	}catch(...) {
		if(appimage) {
			delete appimage;
		}
		qDebug() << "Cannot Resource Extract";
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
			if(!r.second.isEmpty()) {
				break;
			}
		}
		if(entry[0] == "Icon") {
			//// Some crazy Application developers don't follow the 
			//// Rox AppDir format so the icon can be in the top dir
			//// itself. So we will check for that.

			std::vector<char> _iconData;
			QPixmap image;
			QByteArray imageData;	
			bool cannotFind = false;
			try {
				_iconData = res->extract(entry[1] + ".png");
				imageData = QByteArray(_iconData.data(), _iconData.size());
				image.loadFromData(imageData);
			}catch(...) {
				cannotFind = true;
			}

			//// libappimage does not provide anything to check if the given
			//// data is a valid png image, We can do some Qt magic here.
			if(!cannotFind && !image.isNull()) {
				r.second = imageData;
			}else {

			auto paths = res->getIconFilePaths(entry[1]);
			if(paths.size()) {
				auto iconData = res->extract(paths[paths.size()-1]);
				r.second = QByteArray(iconData.data(), iconData.size());
			}
			}
			if(!r.first.isEmpty()) {
				break;
			}
		}
		QCoreApplication::processEvents();
	}

	if(r.first.isEmpty()) {
		r.first = QFileInfo(appimagePath).baseName();
	}

	if(r.second.isEmpty()) {
		/// Give Default Icon
		 QPixmap *pixmap = new QPixmap(":logo.png");
		 QByteArray icon;
		 QBuffer buffer(&icon);
		 buffer.open(QIODevice::WriteOnly);
		 pixmap->save(&buffer, "PNG"); // writes pixmap into bytes in PNG format
		 r.second = icon;
		 delete pixmap;
	}

	delete res;
	delete appimage;
	return r;
}

static QByteArray getByteArrayFromPixmap(const QPixmap &icon) {
	QByteArray iconData;
	QBuffer iconBuffer(&iconData);
	iconBuffer.open(QIODevice::WriteOnly);
	icon.save(&iconBuffer, "PNG");
	return iconData;
}

AppImageUpdaterStandalone::AppImageUpdaterStandalone(QString AppImagePath, int flags, QObject *parent)
    : QObject(parent),
      flags(flags),
      m_AppImagePath(AppImagePath)
{


	m_Updater = new QAppImageUpdate(m_AppImagePath, /*singleThreaded=*/false, /*parent=*/this);
	m_Updater->setShowLog(true);

	auto appimageInformation = getAppImageNameAndIcon(m_AppImagePath);
	if(appimageInformation.first.isEmpty() || 
	   appimageInformation.second.isEmpty()) {
		QPixmap defaultIcon(QString::fromUtf8(":/logo.png"));
		m_Updater->setIcon(getByteArrayFromPixmap(defaultIcon));
		m_Updater->setApplicationName(QFileInfo(m_AppImagePath).baseName());
	} else {
		QPixmap pixmap(100, 100);
		pixmap.loadFromData(appimageInformation.second);
		m_Updater->setIcon(getByteArrayFromPixmap(pixmap));
		m_Updater->setApplicationName(appimageInformation.first);
	}

	m_Updater->setGuiFlag(flags);
	qDebug() << "Flags:: " << flags;

	connect(m_Updater, &QAppImageUpdate::error, this, &AppImageUpdaterStandalone::handleError);
	connect(&m_AuthorizationDialog, &AuthorizationDialog::finished, this, &AppImageUpdaterStandalone::quit);
	connect(m_Updater, &QAppImageUpdate::canceled, this, &AppImageUpdaterStandalone::handleCanceled);
	connect(m_Updater, &QAppImageUpdate::finished, this, &AppImageUpdaterStandalone::handleFinished);
 
}

AppImageUpdaterStandalone::~AppImageUpdaterStandalone()
{
   m_Updater->deleteLater();
   return;
}

void AppImageUpdaterStandalone::init(){
    m_Updater->start(QAppImageUpdate::Action::UpdateWithGUI);   
}

void AppImageUpdaterStandalone::handleError(short errorCode, short action)
{
    Q_UNUSED(action);
    // Ignore all permission errors.
    if(errorCode == QAppImageUpdate::Error::NoReadPermission ||
            errorCode == QAppImageUpdate::Error::NoPermissionToReadSourceFile ||
            errorCode == QAppImageUpdate::Error::NoPermissionToReadWriteTargetFile) {
	    
	    m_AuthorizationDialog.handleAuthorization(QAppImageUpdate::errorCodeToDescriptionString(errorCode),m_AppImagePath);
	    return;
    }
    emit quit();
    return;
}

void AppImageUpdaterStandalone::handleFinished(QJsonObject info, short action)
{
    Q_UNUSED(info);
    Q_UNUSED(action);
    emit quit();
    return;
}

void AppImageUpdaterStandalone::handleCanceled(short action)
{
    Q_UNUSED(action);
    emit quit();
    return;
}
