#ifndef APPIMAGEUPDATER_HPP_INCLUDED
#define APPIMAGEUPDATER_HPP_INCLUDED

#include <QMovie>
#include <QMimeData>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QQueue>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTimer>
#include <QProcess>
#include <QFileInfo>
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <ui_AppImageUpdater.h>
#include <AppImageUpdaterBridge> /* Unofficial AppImage Updater Library for Qt. */
#include <AppImageUpdaterDialog.hpp>

class AppImageUpdater : public QWidget
{
    Q_OBJECT
public:
    AppImageUpdater();
    ~AppImageUpdater();
private Q_SLOTS:
    void updateAppImagesInQueue(void);
    void showAbout(void);
    void showHideWindow(QSystemTrayIcon::ActivationReason);
    void handleFinished(QJsonObject);
    void handleError(QString , short);
    void handleStarted(void);
    void handleCanceled(void);
Q_SIGNALS:
    void quit();
private:
    Ui::MainWidget _pUi;
    QPoint centerPos;
    QAtomicInteger<bool> _bUpdateStarted = false;
    QMessageBox _pAboutMessageBox;
    QString _pCurrentAppImagePath;
    QPixmap _pDropHere,
	    _pDropNorm;
    QIcon _pWindowIcon;
    QQueue<QString> _pAppImagePaths;
    AppImageUpdaterDialog *_pUpdateDialog = nullptr;
    QSystemTrayIcon *_pTIcon = nullptr;
protected:
    void dragMoveEvent(QDragMoveEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
};

#endif // APPIMAGEUPDATER_HPP_INCLUDED
