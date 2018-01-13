#include "AppImageUpdater.hpp"


/*
 * Constructor and Destructor
*/

AppImageUpdater::AppImageUpdater()
{
    Ui = new Ui::AppImageUpdater;
    Bridge = new AIUpdaterBridge(this);
    Bridge->doDebug(true);

    Ui->setupUi(this); // Setup the main gui UI.

    {
        QFile App(QApplication::applicationFilePath());
        App.open(QIODevice::ReadOnly); // assume its successfull because , user cannot open it without reading it.

        Ui->AppInfo->setMinimumSize(QSize(0, 0));
        Ui->AppInfo->setMaximumSize(QSize(779, 168));
        Ui->AppInfo->setPixmap(QPixmap(QString::fromUtf8(":/resources/title.png")));
        Ui->AppInfo->setScaledContents(false);
        Ui->AppInfo->setAlignment(Qt::AlignCenter);

        Ui->VersionInfo->setText("<html><head/><body><p>AppImageUpdater<span style=\" font-weight:600;\"> "+APPIMAGEUPDATER_VERSION_STR+"</span> , Built with <span style=\" font-weight:600;\"> Qt "+QT_VERSION_STR+"</span> with <span style=\" font-weight:600;\">"+QSslSocket::sslLibraryBuildVersionString()+"</span><br><span style=\" font-weight:600;\">AppImage SHA1: </span><span style=\" font-size:11pt;\">"+QString(QCryptographicHash::hash(App.readAll(), QCryptographicHash::Sha1).toHex())+"</span></p></body></html>");
    }

    Ui->MainStack->setCurrentIndex(HOME);

    connect(Bridge, &AIUpdaterBridge::progress, this, &AppImageUpdater::progress);
    connect(Bridge, &AIUpdaterBridge::noUpdatesAvailable, this, &AppImageUpdater::noUpdatesAvailable);
    connect(Bridge, &AIUpdaterBridge::updateFinished, this, &AppImageUpdater::updateFinished);
    connect(Bridge, &AIUpdaterBridge::updatesAvailable, this, &AppImageUpdater::updatesAvailable);
    connect(Bridge, &AIUpdaterBridge::error, this, &AppImageUpdater::handleError);
    connect(Ui->exitOrCancelBtn, &QPushButton::pressed, this, &AppImageUpdater::exit);

    // Connect buttons
    connect(Ui->browseAppImage, &QPushButton::pressed, this,
    [&]() {
        QString filename;
        filename = QFileDialog::getOpenFileName(this, tr("Select AppImage"), QDir::homePath(), tr("AppImages (*.AppImage *.appimage)"));

        if(filename.isEmpty()) {
            return;
        }

        Ui->loader_movie->start();
        Bridge->setAppImageUpdateInformation(filename);

        Ui->MainStack->setCurrentIndex(LOADING);
        return;
    });

    connect(Ui->mainBtn, &QPushButton::pressed, this,
    [&]() {
        Ui->MainStack->setCurrentIndex(HOME);
        Ui->mainBtn->setEnabled(false);
        return;
    });
    return;
}

/* ---- */


// Public Slots

void AppImageUpdater::checkForUpdates(void)
{
    checkForUpdates(QApplication::applicationFilePath());
    return;
}

void AppImageUpdater::checkForUpdates(const QString &filename)
{
    Ui->loader_movie->start();
    Bridge->setAppImageUpdateInformation(filename);
    Ui->MainStack->setCurrentIndex(LOADING);
    return;
}

// Private Slots
void AppImageUpdater::progress(float percent, qint64 bytesRecived, qint64 bytesTotal, double speed,QString unit)
{
    Ui->progress->setValue(percent);
    return;
}

void AppImageUpdater::updateFinished(QString AppImage, QString SHA1)
{
    Ui->exitOrCancelBtn->setText("Exit");
    connect(Ui->exitOrCancelBtn, &QPushButton::pressed, this, &AppImageUpdater::exit);
    disconnect(Ui->exitOrCancelBtn, &QPushButton::pressed, this, &AppImageUpdater::cancel);
    Ui->sckFoot->setText("<html><head/><body><p>Updated <span style=\" font-weight:600;\">"+ QFileInfo(AppImage).fileName() +"</span> ( SHA1: "+ SHA1 +")</p></body></html>");
    Ui->mainBtn->setEnabled(true);
    Ui->loader_movie->stop();
    Ui->MainStack->setCurrentIndex(SUCCESS);
    return;
}

void AppImageUpdater::noUpdatesAvailable(QString AppImage, QString oldSHA1)
{
    if(AppImage == QApplication::applicationFilePath()) {
        Ui->mainBtn->setEnabled(false);
        Ui->loader_movie->stop();
        Ui->MainStack->setCurrentIndex(HOME);
        return;
    }
    Ui->sckFoot->setText("<html><head/><body><p>Already Uptodate for <span style=\" font-weight:600;\">"+ QFileInfo(AppImage).fileName() +"</span> ( SHA1: "+ oldSHA1 +")</p></body></html>");
    Ui->mainBtn->setEnabled(true);
    Ui->loader_movie->stop();
    Ui->MainStack->setCurrentIndex(SUCCESS);
    return;
}

void AppImageUpdater::updatesAvailable(QString AppImage, QString newSHA1)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "AppImage Updater",
                                         "A New Version of "+QFileInfo(AppImage).fileName() +" is available , Do you want to Update ?\n",
                                         QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                         QMessageBox::Yes);

    if (resBtn != QMessageBox::Yes) {
        Ui->loader_movie->stop();
        Ui->MainStack->setCurrentIndex(HOME);
        return;
    }

    QFile App(AppImage);
    App.open(QIODevice::ReadOnly);

    Ui->AppImagePath->setText("<html><head/><body><p>Updating AppImage...<span style=\" font-weight:600;\"> "+AppImage+"</span></p><p><span style=\" font-weight:600;\">Local SHA1 : </span>"+QString(QCryptographicHash::hash(App.readAll(), QCryptographicHash::Sha1).toHex())+"</p><p><span style=\" font-weight:600;\">Upstream SHA1: </span>"+ newSHA1 +"</p><p><br/>Please wait as we are porting your <span style=\" font-weight:600;\">AppImage </span>to the<span style=\" font-weight:600;\"> future</span>. This might take some time so <span style=\" font-weight:600;\">have a Tea!</span></p></body></html>");
    Ui->loader_movie->stop();
    Ui->MainStack->setCurrentIndex(UPDATING);

    Ui->exitOrCancelBtn->setText("Cancel");
    disconnect(Ui->exitOrCancelBtn, &QPushButton::pressed, this, &AppImageUpdater::exit);
    connect(Ui->exitOrCancelBtn, &QPushButton::pressed, this, &AppImageUpdater::cancel);

    Bridge->startUpdating();
    return;
}

void AppImageUpdater::handleError(QString AppImage, short errorCode)
{
    QString whatString = "<html><head/><body><p>";
    QString file = QFileInfo(AppImage).fileName();
    // Get the cause
    switch(errorCode) {
    case AIUpdaterBridge::UNABLE_TO_GET_APPIMAGE_INFORMATION:
        whatString += "Unable to extract update information from <b>" + file + "</b>";
        break;
    case AIUpdaterBridge::APPIMAGE_PATH_NOT_GIVEN:
        whatString += "No AppImage given to process.";
        break;
    case AIUpdaterBridge::NETWORK_ERROR:
        whatString += "Unable to update <b>" + file + "</b> because of unknow network interruption.";
        break;
    case AIUpdaterBridge::CANNOT_FIND_GITHUB_ASSET:
        whatString += "Cannot get the github asset for <b>" + file + "</b>!";
        break;
    case AIUpdaterBridge::ZSYNC_HEADER_INVALID:
        whatString += "Failed to update <b>" + file + "</b> because the remote zsync file has invalid zsync header!";
        break;
    case AIUpdaterBridge::APPIMAGE_NOT_FOUND:
        whatString += "Unable to locate <b>" + AppImage + "</b>!";
        break;
    case AIUpdaterBridge::FAILED_TO_OPEN_ZSYNC_HANDLE:
        whatString += "Failed to open zsync handler for <b>" + file + "</b>. Try running AppImage Updater with admin rights.";
        break;
    case AIUpdaterBridge::ZSYNC_RANGE_FETCH_FAILED:
        whatString += "Failed to fetch the range for <b>" + file + "</b>. Zsync fatal error.";
        break;
    case AIUpdaterBridge::ZSYNC_RECIEVE_FAILED:
        whatString += "Failed to recieve update for <b>" + file + "</b>. Zsync recieve failed.";
        break;
    case AIUpdaterBridge::CANNOT_FIND_BINTRAY_PACKAGE:
        whatString += "Cannot locate bintray package for <b>" + file + "</b>. Please contact the author!";
        break;
    case AIUpdaterBridge::POST_INSTALLATION_FAILED:
        whatString += "Post installation failed , make sure you have write permissions!";
        break;
    case AIUpdaterBridge::UPDATE_INTEGRITY_FAILED:
        whatString += "SHA1 does not match the downloaded file , Please notify the author. Update failed for <b>"+file+"</b>.";
        break;
    case AIUpdaterBridge::FAILED_TO_RENAME_TEMPFILE:
        whatString += "Failed to rename temporary file for <b>" + file + "</b>. Post installation failed.";
        break;
    case AIUpdaterBridge::BAD_ALLOC:
        whatString += "Out of Memory!";
        break;
    default:
        whatString += "Failed to Update <b>" + file + "</b>. Uncaught error!";
        break;
    }
    whatString += "</p></body></html>";
    Ui->mainBtn->setEnabled(true);
    Ui->failFoot->setText(whatString);
    Ui->MainStack->setCurrentIndex(FAILED);
    return;
}

void AppImageUpdater::cancel(void)
{
    connect(Bridge, &AIUpdaterBridge::stopped, this,
    [&]() {
        Ui->exitOrCancelBtn->setText("Exit");
        connect(Ui->exitOrCancelBtn, &QPushButton::pressed, this, &AppImageUpdater::exit);
        disconnect(Ui->exitOrCancelBtn, &QPushButton::pressed, this, &AppImageUpdater::cancel);
        Ui->loader_movie->stop();
        Ui->MainStack->setCurrentIndex(HOME);
        return;
    });
    Ui->loader_movie->start();
    Ui->MainStack->setCurrentIndex(LOADING);
    Bridge->stopUpdating();
    return;
}

void AppImageUpdater::exit(void)
{
    emit safeToCloseApplication();
    return;
}
