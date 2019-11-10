#ifndef APPIMAGE_UPDATER_STANDALONE_HPP_INCLUDED
#define APPIMAGE_UPDATER_STANDALONE_HPP_INCLUDED
#include <AppImageUpdaterBridge>
#include <AppImageUpdaterDialog>
#include <QObject>
#include <QJsonObject>

class AppImageUpdaterStandalone : public QObject
{
    Q_OBJECT

    int flags;
    QString m_AppImagePath;
    AppImageUpdaterBridge::AppImageDeltaRevisioner *m_Updater = nullptr;
    AppImageUpdaterBridge::AppImageUpdaterDialog *_pUpdateDialog = nullptr;
public:
    AppImageUpdaterStandalone(QString, int, QObject *parent = nullptr);
    ~AppImageUpdaterStandalone();
private Q_SLOTS:
    void handleError(QString, short);
    void handleFinished(QJsonObject);
    void handleCanceled(void);
    void handleAppImageInformation(QJsonObject);
Q_SIGNALS:
    void quit();
};

#endif // APPIMAGE_UPDATER_STANDALONE_HPP_INCLUDED
