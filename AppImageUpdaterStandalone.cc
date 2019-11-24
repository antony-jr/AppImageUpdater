#include <AppImageUpdaterStandalone.hpp>
#include <QFileInfo>
#include <QScreen>
#include <QMessageBox>

using AppImageUpdaterBridge::AppImageUpdaterDialog;
using AppImageUpdaterBridge::AppImageDeltaRevisioner;

AppImageUpdaterStandalone::AppImageUpdaterStandalone(QString AppImagePath, int flags, QObject *parent)
    : QObject(parent),
      flags(flags),
      m_AppImagePath(AppImagePath)
{

	m_Updater = new AppImageDeltaRevisioner(AppImagePath , this);
	m_Updater->setShowLog(true);
	connect(m_Updater,
		    &AppImageDeltaRevisioner::embededInformation, 
		    this, &AppImageUpdaterStandalone::handleAppImageInformation,
		    Qt::UniqueConnection);
	connect(m_Updater, &AppImageDeltaRevisioner::error, this, &AppImageUpdaterStandalone::handleError);
}

AppImageUpdaterStandalone::~AppImageUpdaterStandalone()
{
   if(_pUpdateDialog){
	_pUpdateDialog->hide();
	_pUpdateDialog->deleteLater();
   }
   return;
}

void AppImageUpdaterStandalone::init(){
    m_Updater->getAppImageEmbededInformation();   
}

void AppImageUpdaterStandalone::handleAppImageInformation(QJsonObject info){
    QString applicationName;
    QJsonObject updInfo = (info["UpdateInformation"]).toObject();
    QString transportType = (updInfo["transport"]).toString();

    _pUpdateDialog = new AppImageUpdaterDialog(QPixmap(QString::fromUtf8(":/default_icon.png")), nullptr,
		    (flags ^ AppImageUpdaterDialog::ShowErrorDialog) | 
		    AppImageUpdaterDialog::AlertWhenAuthorizationIsRequired);
    _pUpdateDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    _pUpdateDialog->move(QGuiApplication::primaryScreen()->geometry().center() - _pUpdateDialog->rect().center());

    if(transportType == QString::fromUtf8("gh-releases-zsync") ||
       transportType == QString::fromUtf8("bintray-zsync")){
	    applicationName = (updInfo["repo"]).toString();
    }else{
	    applicationName = QFileInfo(m_AppImagePath).baseName();
    }

    _pUpdateDialog->init(m_Updater, applicationName);

    //Program logic.
    connect(&_pAuthorizationDialog, &AuthorizationDialog::started, _pUpdateDialog, &QDialog::hide, Qt::DirectConnection);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::requiresAuthorization, &_pAuthorizationDialog, &AuthorizationDialog::handleAuthorization);
    connect(&_pAuthorizationDialog, &AuthorizationDialog::finished, this, &AppImageUpdaterStandalone::quit);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::canceled, this, &AppImageUpdaterStandalone::handleCanceled);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::finished, this, &AppImageUpdaterStandalone::handleFinished);
    return;

}



void AppImageUpdaterStandalone::handleError(short errorCode)
{
    // Ignore all permission errors.
    if(errorCode == AppImageUpdaterBridge::NoReadPermission ||
            errorCode == AppImageUpdaterBridge::NoPermissionToReadSourceFile ||
            errorCode == AppImageUpdaterBridge::NoPermissionToReadWriteTargetFile) {
	    return;
    }
 
    if(flags & AppImageUpdaterDialog::ShowErrorDialog){
    QMessageBox box;
    box.setWindowTitle(QString::fromUtf8("Update Failed"));
    box.setIcon(QMessageBox::Critical);
    box.setText(QString::fromUtf8("Update failed for '") + 
		QFileInfo(m_AppImagePath).fileName() + 
		QString::fromUtf8("': ") + AppImageUpdaterBridge::errorCodeToDescriptionString(errorCode));
    box.exec(); 
 
    }
    emit quit();
    return;
}

void AppImageUpdaterStandalone::handleFinished(QJsonObject info)
{
    (void)info;
    emit quit();
    return;
}

void AppImageUpdaterStandalone::handleCanceled(void)
{
    if(_pUpdateDialog){
    	_pUpdateDialog->hide();
	m_Updater->deleteLater();
    }
    emit quit();
    return;
}
