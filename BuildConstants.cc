#include <BuildConstants.hpp>
#include <libtorrent/version.hpp>
#include <QAppImageUpdate>

#ifndef APPIMAGE_UPDATER_VERSION
#define APPIMAGE_UPDATER_VERSION "2"
#endif
#ifndef APPIMAGE_UPDATER_COMMIT
#define APPIMAGE_UPDATER_COMMIT "none"
#endif
#ifndef APPIMAGE_UPDATER_BUILD_NO
#define APPIMAGE_UPDATER_BUILD_NO "1"
#endif
#ifndef APPIMAGE_UPDATER_BUILD_TIME
#define APPIMAGE_UPDATER_BUILD_TIME __DATE__
#endif
 

QString BuildConstants::getCommit() {
	return QString::fromUtf8(APPIMAGE_UPDATER_COMMIT);
}

QString BuildConstants::getBuildNo() { 
	return QString::fromUtf8(APPIMAGE_UPDATER_BUILD_NO);
}

QString BuildConstants::getBuildTime() { 
	return QString::fromUtf8(APPIMAGE_UPDATER_BUILD_TIME);
}

QString BuildConstants::getQAppImageUpdateVersion() {
	return QAppImageUpdate::versionString();
}

QString BuildConstants::getTorrentVersion() { 
	return QString::fromUtf8(LIBTORRENT_VERSION);
}

QString BuildConstants::getVersion() {
	return QString::fromUtf8(APPIMAGE_UPDATER_VERSION);
}
