#include <AppImageUpdater.hpp>
#include <QScreen>
#include <QMenu>
#include <QAction>

using namespace AppImageUpdaterBridge;

#define SELF_UPDATER_IDLE_TIME 4000 // 4 Secs for now

/* DR => Dereference pointers. */
#define DR(pointer) (*pointer)

/* Access UserInterface. */
#define AUI(Object) DR(_pUi.Object)

/* AppImage Delta Revisioner Signals. */
#define ADRUA_SIGNAL &AppImageDeltaRevisioner::updateAvailable

AppImageUpdater::AppImageUpdater(bool minimized, QWidget *parent)
    : QWidget(parent, Qt::WindowStaysOnTopHint)
{
    _pUi.setupUi(this);
    centerPos = QGuiApplication::primaryScreen()->geometry().center();
    setAcceptDrops(true);

    /* Construct tray icon. */
    _pTIcon = new QSystemTrayIcon(this);
    _pTIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/logo.png"))));
    // Menu context for the tray icon
    QMenu *menu = new QMenu(this);
    menu->addAction(QString::fromUtf8("Show / Hide"),
		    this,
		    &AppImageUpdater::showHide);
    menu->addAction(QString::fromUtf8("Quit"),
		    this,
		    &AppImageUpdater::quit);

    _pTIcon->setContextMenu(menu);
    connect(_pTIcon, &QSystemTrayIcon::activated, this, &AppImageUpdater::showHideWindow);
    _pTIcon->show();

    /* Do not show the app if it ran on startup. */
    if(_pSettings.isRunOnStartup() && minimized) {
        this->hide();
    } else {
        this->move(centerPos - this->rect().center());
        this->show();
    }

    /* This is oftenly used pixmap , Thus to reduce overhead we are caching it. */
    _pDropHere = QPixmap(QString::fromUtf8(":/dotted_square_drop.png"));
    _pDropNorm = QPixmap(QString::fromUtf8(":/dotted_square.png"));
    _pWindowIcon = QIcon(QPixmap(QString::fromUtf8(":/logo.png")));

    /* Build about message box. */
    {
        QString AboutText;
        AboutText.append("AppImage Updater version ");
        AboutText.append(QString(APPIMAGE_UPDATER_VERSION));
        AboutText.append(" (commit ");
        AboutText.append(QString(APPIMAGE_UPDATER_COMMIT));
        AboutText.append(") , Build  ");
        AboutText.append(QString(APPIMAGE_UPDATER_BUILD_NO));
        AboutText.append(" , Built on ");
        AboutText.append(QString(APPIMAGE_UPDATER_BUILD_TIME));
        AboutText.append(".<br><br>");
        AboutText.append("The GNU Lesser General Public License v3.<br>");
        AboutText.append("Copyright &#169; 2018 , Antony Jr.<br><br>");
        AboutText.append("Icons by <a href=\"https://flaticon.com\">FlatIcon</a>.<br>");
        AboutText.append("Released under Creative Commons.<br><br>");
        AboutText.append("Report issues at <a href=\"https://github.com/antony-jr/AppImageUpdater\">github</a>.<br>");
        AboutText.append("This project is maintained by <a href=\"https://github.com/antony-jr\">Antony Jr</a>.");
        _pAboutMessageBox.setTextFormat(Qt::RichText);
        _pAboutMessageBox.setText(AboutText);
        _pAboutMessageBox.setIconPixmap(QPixmap(QString::fromUtf8(":/logo.png")).scaled( 128, 128, Qt::KeepAspectRatio));
    }


    /* Check for updates. */
    _bUpdateStarted = true;

    m_Updater = new AppImageDeltaRevisioner(/*single threaded=*/false, this);
    m_Updater->setShowLog(true);

    int flags =  0; 
    if(_pSettings.isShowUpdateDialogs()){
	    flags = (AppImageUpdaterDialog::Default ^ 
	             AppImageUpdaterDialog::ShowErrorDialog ^ 
		     AppImageUpdaterDialog::ShowBeforeProgress ^ 
		     AppImageUpdaterDialog::NotifyWhenNoUpdateIsAvailable) | AppImageUpdaterDialog::AlertWhenAuthorizationIsRequired;
    }
   
    _pUpdateDialog = new AppImageUpdaterDialog(QPixmap(QString::fromUtf8(":/default_icon.png")),this,flags);
    _pUpdateDialog->move(centerPos - _pUpdateDialog->rect().center());
    
    /* Special connect */
    connect(_pUpdateDialog, &AppImageUpdaterDialog::quit, this, &AppImageUpdater::quit, Qt::DirectConnection);

    _pUpdateDialog->init(m_Updater);

    /* Connect buttons. */
    connect(_pUi.settingsBtn, &QPushButton::pressed, &_pSettings, &QDialog::exec);
    connect(_pUi.exitBtn, &QPushButton::pressed, this, &AppImageUpdater::quit, Qt::DirectConnection);
    connect(_pUi.aboutBtn, &QPushButton::pressed, this, &AppImageUpdater::showAbout);

    /* Program logic. */
    connect(_pUpdateDialog, &AppImageUpdaterDialog::requiresAuthorization, &_pAuthorizationDialog, &AuthorizationDialog::handleAuthorization);
    connect(&_pAuthorizationDialog, &AuthorizationDialog::finished, this, &AppImageUpdater::handleAuthorizationFinished, Qt::UniqueConnection);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::started, this, &AppImageUpdater::handleStarted);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::canceled, this, &AppImageUpdater::handleCanceled);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::error, this, &AppImageUpdater::handleError);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::finished, this, &AppImageUpdater::handleFinished);
    return;
}

AppImageUpdater::~AppImageUpdater()
{

    return;
}

void AppImageUpdater::gracefulShow(void)
{
    this->hide();
    this->move(centerPos - this->rect().center());
    this->show();
    if(_pSettings.isShowSystemTrayNotificationMessages()) {
        _pTIcon->showMessage(QString::fromUtf8("AppImage Updater"),
                             QString::fromUtf8("The Application is running already , Please close this instance to start a new one."));
    }
    return;
}

void AppImageUpdater::closeEvent(QCloseEvent *e)
{
    this->hide();
    if(_pSettings.isShowSystemTrayNotificationMessages()) {
        _pTIcon->showMessage(QString::fromUtf8("AppImage Updater"),
                             QString::fromUtf8("The Application is running in the background , Click on the system tray icon to use it."));
    }
    e->ignore();
    return;
}

void AppImageUpdater::handleStarted(void)
{
    _bUpdateStarted = true;
    return;
}

void AppImageUpdater::handleCanceled(void)
{
    _bUpdateStarted = false;
    updateAppImagesInQueue();
    return;
}


void AppImageUpdater::handleError(QString eStr, short errorCode)
{
    (void)eStr;
    (void)errorCode;
    _bUpdateStarted = false;
    updateAppImagesInQueue();
    return;
}

void AppImageUpdater::handleFinished(QJsonObject info)
{
    auto r = (AppImageUpdaterDialog*)QObject::sender();
    r->deleteLater();
    (void)info;
    _bUpdateStarted = false;
    updateAppImagesInQueue();
    return;
}

void AppImageUpdater::handleAuthorizationFinished(QJsonObject info)
{
    (void)info;
    _bUpdateStarted = false;
    updateAppImagesInQueue();
    return;
}

/* Show hide window. */
void AppImageUpdater::showHide(void){
        this->move(centerPos - this->rect().center());
        if(this->isVisible()) {
            this->hide();
            if(_pSettings.isShowSystemTrayNotificationMessages()) {
                _pTIcon->showMessage(QString::fromUtf8("AppImage Updater"),
                                     QString::fromUtf8("Click on the system tray icon to use AppImage Updater."));
            }
        } else {
            this->show();
        }

}
void AppImageUpdater::showHideWindow(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger) {
        this->move(centerPos - this->rect().center());
        if(this->isVisible()) {
            this->hide();
            if(_pSettings.isShowSystemTrayNotificationMessages()) {
                _pTIcon->showMessage(QString::fromUtf8("AppImage Updater"),
                                     QString::fromUtf8("Click on the system tray icon to use AppImage Updater."));
            }
        } else {
            this->show();
        }
    }
    return;
}

/* Show about dialog box. */
void AppImageUpdater::showAbout(void)
{
    _pAboutMessageBox.exec();
    return;
}

void AppImageUpdater::handleAppImageInformation(QJsonObject info){
    int flags =  0; 
    QString applicationName;
    QJsonObject updInfo = (info["UpdateInformation"]).toObject();
    QString transportType = (updInfo["transport"]).toString();

 
    if(_pSettings.isShowUpdateDialogs()){
	    flags = (AppImageUpdaterDialog::Default ^ AppImageUpdaterDialog::ShowBeforeProgress) | 
		    AppImageUpdaterDialog::AlertWhenAuthorizationIsRequired;
    }

    _pUpdateDialog = new AppImageUpdaterDialog(QPixmap(QString::fromUtf8(":/default_icon.png")),this,flags);
    _pUpdateDialog->move(centerPos - _pUpdateDialog->rect().center());

    if(transportType == QString::fromUtf8("gh-releases-zsync") ||
       transportType == QString::fromUtf8("bintray-zsync")){
	    applicationName = (updInfo["repo"]).toString();
    }else{
	    applicationName = QFileInfo(_pCurrentAppImagePath).baseName();
    }

    _pUpdateDialog->init(m_Updater, applicationName);

    /* Program logic. */
    connect(&_pAuthorizationDialog, &AuthorizationDialog::started, _pUpdateDialog, &QDialog::hide, Qt::DirectConnection);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::requiresAuthorization, &_pAuthorizationDialog, &AuthorizationDialog::handleAuthorization);
    connect(&_pAuthorizationDialog, &AuthorizationDialog::finished, this, &AppImageUpdater::handleAuthorizationFinished, Qt::UniqueConnection);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::started, this, &AppImageUpdater::handleStarted);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::canceled, this, &AppImageUpdater::handleCanceled);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::error, this, &AppImageUpdater::handleError);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::finished, this, &AppImageUpdater::handleFinished);
    return;

}

/* Updates AppImages in queue. */
void AppImageUpdater::updateAppImagesInQueue(void)
{
    if(_bUpdateStarted == true) {
        return;
    }
    if(_pAppImagePaths.isEmpty()) {
        AUI(statusLbl).setText(QString::fromUtf8("Nothing is Queued for Update."));
        if(_pSettings.isShowSystemTrayNotificationMessages()) {
            _pTIcon->showMessage("AppImage Updater", "Finished all queued updates.");
        }
        return;
    }

    _bUpdateStarted = true;
    _pCurrentAppImagePath = _pAppImagePaths.dequeue();

    const QString msg("Updating %1 , Queued %2 AppImage(s) for Update.");
    QString AppImageSName = QFileInfo(_pCurrentAppImagePath).fileName();
    if(AppImageSName.length() > 20) {
        AppImageSName = AppImageSName.left(18);
        AppImageSName += QString::fromUtf8("..");
    }
    AUI(statusLbl).setText(msg.arg(AppImageSName).arg(_pAppImagePaths.size()));

    QObject::connect(m_Updater,
		    &AppImageDeltaRevisioner::embededInformation, 
		    this, &AppImageUpdater::handleAppImageInformation,
		    Qt::UniqueConnection);

    m_Updater->setAppImage(_pCurrentAppImagePath);
    m_Updater->getAppImageEmbededInformation();
    return;
}

/* drop move event override. */
void AppImageUpdater::dragMoveEvent(QDragMoveEvent *e)
{
    e->accept();
    /* Lets react to the user's drag. */
    AUI(dragAndDropArea).setPixmap(_pDropHere);
    return;
}

/* drop leave event override. */
void AppImageUpdater::dragLeaveEvent(QDragLeaveEvent *e)
{
    Q_UNUSED(e);
    /* Reset back to normal. */
    AUI(dragAndDropArea).setPixmap(_pDropNorm);
    return;
}

/* drop enter event override. */
void AppImageUpdater::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
    return;
}

/* handle drop event. */
void AppImageUpdater::dropEvent(QDropEvent *e)
{
    /* Reset back to normal. */
    AUI(dragAndDropArea).setPixmap(_pDropNorm);

    /* Notification message template. */
    const QString statusMsg("%1 AppImage(s) Queued for Update.");
    const QString msg("'%1' is queued for update.");
    const QString msgDir("The contents of '%1' has been recursively queued for update!");
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        if(QFileInfo(fileName).isDir()) {
            QDirIterator dirIt(fileName,
                               QStringList() << "*.AppImage"
                               << "*.desktop",
                               QDir::Files,
                               QDirIterator::Subdirectories);
            while (dirIt.hasNext()) {
                auto file = dirIt.next();
                _pAppImagePaths.enqueue(file);
                QCoreApplication::processEvents();
            }
            if(_pSettings.isShowSystemTrayNotificationMessages()) {
                _pTIcon->showMessage(QString::fromUtf8("AppImage Updater"), msgDir.arg(fileName));
            }
            continue;
        }
        _pAppImagePaths.enqueue(fileName);
        if(_pSettings.isShowSystemTrayNotificationMessages()) {
            _pTIcon->showMessage(QString::fromUtf8("AppImage Updater"), msg.arg(fileName));
        }
        AUI(statusLbl).setText(statusMsg.arg(_pAppImagePaths.size()));
        QCoreApplication::processEvents();
    }

    if(_bUpdateStarted != true) {
        updateAppImagesInQueue();
    }
    return;
}
