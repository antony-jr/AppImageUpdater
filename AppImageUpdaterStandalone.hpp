#ifndef APPIMAGE_UPDATER_STANDALONE_HPP_INCLUDED
#define APPIMAGE_UPDATER_STANDALONE_HPP_INCLUDED
#include <QAppImageUpdate>
#include <QObject>
#include <QJsonObject>

#include <AuthorizationDialog.hpp>

class AppImageUpdaterStandalone : public QObject
{
    Q_OBJECT

    int flags;
    QString m_AppImagePath;
    QAppImageUpdate *m_Updater;
    AuthorizationDialog _pAuthorizationDialog;
public:
    AppImageUpdaterStandalone(QString, int, QObject *parent = nullptr);
    ~AppImageUpdaterStandalone();
    
    void init();
private Q_SLOTS:
    void handleError(short);
    void handleFinished(QJsonObject, short);
    void handleCanceled(short);
    void handleAppImageInformation(QJsonObject, short);
Q_SIGNALS:
    void quit();
};

#endif // APPIMAGE_UPDATER_STANDALONE_HPP_INCLUDED
