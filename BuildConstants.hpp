#ifndef BUILD_CONSTANTS_HPP_INCLUDED 
#define BUILD_CONSTANTS_HPP_INCLUDED
#include <QObject>
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
 

class BuildConstants : public QObject {
    Q_OBJECT
  public:
    Q_INVOKABLE QString getCommit() {
	return QString::fromUtf8(APPIMAGE_UPDATER_COMMIT);
    }
    Q_INVOKABLE QString getBuildNo() { 
	return QString::fromUtf8(APPIMAGE_UPDATER_BUILD_NO);
    }
    Q_INVOKABLE QString getBuildTime() { 
	return QString::fromUtf8(APPIMAGE_UPDATER_BUILD_TIME);
    }
    Q_INVOKABLE QString getQAppImageUpdateVersion() {
	return QAppImageUpdate::versionString();
    }
    Q_INVOKABLE QString getTorrentVersion() { 
	return QString::fromUtf8(LIBTORRENT_VERSION);
    }
    Q_INVOKABLE QString getVersion() {
	return QString::fromUtf8(APPIMAGE_UPDATER_VERSION);
    }
};



#endif // BUILD_CONSTANTS_HPP_INCLUDED
