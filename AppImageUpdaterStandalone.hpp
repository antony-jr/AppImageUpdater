#ifndef APPIMAGE_UPDATER_STANDALONE_HPP_INCLUDED
#define APPIMAGE_UPDATER_STANDALONE_HPP_INCLUDED
#include <AppImageUpdaterBridge>
#include <AppImageUpdaterDialog>
#include <QObject>
#include <QJsonObject>

#include <AuthorizationDialog.hpp>

class AppImageUpdaterStandalone : public QObject
{
    Q_OBJECT

    int flags;
    QString m_AppImagePath;
    AppImageUpdaterBridge::AppImageDeltaRevisioner *m_Updater = nullptr;
    AppImageUpdaterBridge::AppImageUpdaterDialog *_pUpdateDialog = nullptr;
    AuthorizationDialog _pAuthorizationDialog;
public:
    AppImageUpdaterStandalone(QString, int, QObject *parent = nullptr);
    ~AppImageUpdaterStandalone();
    
    void init();
private Q_SLOTS:
    void handleError(short);
    void handleFinished(QJsonObject);
    void handleCanceled(void);
    void handleAppImageInformation(QJsonObject);
Q_SIGNALS:
    void quit();
};

#endif // APPIMAGE_UPDATER_STANDALONE_HPP_INCLUDED
