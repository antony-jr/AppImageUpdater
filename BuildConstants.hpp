#ifndef BUILD_CONSTANTS_HPP_INCLUDED 
#define BUILD_CONSTANTS_HPP_INCLUDED
#include <QObject>

class BuildConstants : public QObject {
    Q_OBJECT
  public:
    Q_INVOKABLE QString getCommit();
    Q_INVOKABLE QString getBuildNo(); 
    Q_INVOKABLE QString getBuildTime(); 
    Q_INVOKABLE QString getQAppImageUpdateVersion();
    Q_INVOKABLE QString getTorrentVersion();
    Q_INVOKABLE QString getVersion();
};

#endif // BUILD_CONSTANTS_HPP_INCLUDED
