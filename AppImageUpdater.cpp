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
    QFile App(QFileInfo(QApplication::applicationFilePath()).fileName());
    App.open(QIODevice::ReadOnly); // assume its successfull because , user cannot open it without reading it.
    Ui->VersionInfo->setText("<html><head/><body><p>AppImageUpdater<span style=\" font-weight:600;\"> "+APPIMAGEUPDATER_VERSION_STR+"</span> , Built with <span style=\" font-weight:600;\"> Qt "+QT_VERSION_STR+"</span> with <span style=\" font-weight:600;\">"+QSslSocket::sslLibraryBuildVersionString()+"</span>.</p></body></html>");
    Ui->AppSha1->setText("<html><head/><body><p><span style=\" font-weight:600;\">AppImage SHA1: </span><span style=\" font-size:11pt;\">"+QString(QCryptographicHash::hash(App.readAll(), QCryptographicHash::Sha1).toHex())+"</span></p></body></html>");
    }
    Ui->MainStack->setCurrentIndex(HOME);
    
    connect(Bridge , &AIUpdaterBridge::progress ,
    [&](float percent, qint64 bytesRecived , qint64 bytesTotal , double speed , const QString unit){
        Ui->progress->setValue(percent);
        return;
    });
    
    connect(Bridge , &AIUpdaterBridge::noUpdatesAvailable,
    [&](QString AppImage , QString oldSHA1){
        Ui->sckFoot->setText("<html><head/><body><p>Updated <span style=\" font-weight:600;\">"+ AppImage +"</span> ( SHA1: "+ oldSHA1 +")</p></body></html>");
        Ui->loader_movie->stop();
        Ui->MainStack->setCurrentIndex(SUCCESS);
        return;
    });
    
    connect(Bridge , &AIUpdaterBridge::updateFinished,
    [&](QString AppImage , QString SHA1){
        Ui->sckFoot->setText("<html><head/><body><p>Updated <span style=\" font-weight:600;\">"+ AppImage +"</span> ( SHA1: "+ SHA1 +")</p></body></html>");
        Ui->MainStack->setCurrentIndex(SUCCESS);
        return;
    });
    
    connect(Bridge , &AIUpdaterBridge::updatesAvailable ,
    [&](QString AppImage , QString newSHA1){
        QFile App(AppImage);
        App.open(QIODevice::ReadOnly);
        Ui->AppImagePath->setText("<html><head/><body><p>Updating AppImage...<span style=\" font-weight:600;\"> "+AppImage+"</span></p><p><span style=\" font-weight:600;\">Local SHA1 : </span>"+QString(QCryptographicHash::hash(App.readAll(), QCryptographicHash::Sha1).toHex())+"</p><p><span style=\" font-weight:600;\">Upstream SHA1: </span>"+ newSHA1 +"</p><p><br/>Please wait as we are porting your <span style=\" font-weight:600;\">AppImage </span>to the<span style=\" font-weight:600;\"> future</span>. This might take some time so <span style=\" font-weight:600;\">have a Tea!</span></p></body></html>");
        Ui->loader_movie->stop();
        Ui->MainStack->setCurrentIndex(UPDATING);
        Bridge->startUpdating();
        return;
    });
    
    connect(Ui->browseAppImage , &QPushButton::pressed , this ,
    [&](){
        QString filename;
        filename = QFileDialog::getOpenFileName(this, tr("Select AppImage"), QDir::homePath() , tr("AppImages (*.AppImage *.appimage)"));
        
        Ui->loader_movie->start();
        Bridge->setAppImageUpdateInformation(filename);
        
        Ui->MainStack->setCurrentIndex(LOADING);
        return;
    });
}

/* ---- */
