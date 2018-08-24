#ifndef APPIMAGEUPDATER_HPP_INCLUDED
#define APPIMAGEUPDATER_HPP_INCLUDED

#include <QMovie>
#include <QMimeData>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QQueue>
#include <QMessageBox>
#include <ui_AppImageUpdater.h>
#include <AppImageUpdaterBridge> /* Unofficial AppImage Updater Library for Qt. */

class AppImageUpdater : public QWidget
{
    Q_OBJECT
public:
    AppImageUpdater();
    ~AppImageUpdater();
private Q_SLOTS:
    void showAbout(void);
    void handleError(short);
    void updateAppImagesInQueue(void);
    void selfUpdateAvailable(bool , QString);
Q_SIGNALS:
    void quit();
private:
    /* Indices of the stacked widget. */
    enum {
        INPUT,
        CHECKING,
        UPDATING,
        RESULT
    };
    Ui::MainWidget _pUi;
    QMessageBox _pAboutMessageBox;
    QString _pCurrentAppImagePath;
    QPixmap _pDropHere,
	    _pDropNorm;
    QQueue<QString> _pAppImagePaths;
    AppImageUpdaterBridge::AppImageDeltaRevisioner *_pAppImageRevisioner = nullptr;
    AppImageUpdaterBridge::AppImageDeltaRevisioner *_pSelfRevisioner = nullptr;
protected:
    void dragMoveEvent(QDragMoveEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
};

#endif // APPIMAGEUPDATER_HPP_INCLUDED
