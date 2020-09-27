#include <AppImageUpdaterStandalone.hpp>
#include <QFileInfo>
#include <QScreen>
#include <QMessageBox>
#include <QBuffer>

AppImageUpdaterStandalone::AppImageUpdaterStandalone(QString AppImagePath, int flags, QObject *parent)
    : QObject(parent),
      flags(flags),
      m_AppImagePath(AppImagePath)
{


	m_Updater = new QAppImageUpdate(AppImagePath , this);
	m_Updater->setShowLog(true);
	
	 
    	QByteArray iconData;
    	QBuffer iconBuffer(&iconData);
    	QPixmap defaultIcon(QString::fromUtf8(":/default_icon.png"));
    	iconBuffer.open(QIODevice::WriteOnly);
    	defaultIcon.save(&iconBuffer, "PNG");
	m_Updater->setIcon(iconData);

	m_Updater->setAppImage(m_AppImagePath);

	connect(m_Updater,
		    &QAppImageUpdate::finished, 
		    this, &AppImageUpdaterStandalone::handleAppImageInformation,
		    Qt::UniqueConnection);
	connect(m_Updater, &QAppImageUpdate::error, this, &AppImageUpdaterStandalone::handleError);
}

AppImageUpdaterStandalone::~AppImageUpdaterStandalone()
{
   m_Updater->deleteLater();
   return;
}

void AppImageUpdaterStandalone::init(){
    m_Updater->start(QAppImageUpdate::Action::GetEmbeddedInfo);   
}

void AppImageUpdaterStandalone::handleAppImageInformation(QJsonObject info, short action){
    if(action != QAppImageUpdate::Action::GetEmbeddedInfo) {
	    return;
    }
    QString applicationName;
    QJsonObject updInfo = (info["UpdateInformation"]).toObject();
    QString transportType = (updInfo["transport"]).toString();

    flags |= QAppImageUpdate::GuiFlag::NoShowErrorDialogOnPermissionErrors;


    if(transportType == QString::fromUtf8("gh-releases-zsync") ||
       transportType == QString::fromUtf8("bintray-zsync")){
	    applicationName = (updInfo["repo"]).toString();
    }else{
	    applicationName = QFileInfo(m_AppImagePath).baseName();
    }

    m_Updater->setApplicationName(applicationName);

    //Program logic.
    connect(&_pAuthorizationDialog, &AuthorizationDialog::finished, this, &AppImageUpdaterStandalone::quit);
    connect(m_Updater, &QAppImageUpdate::canceled, this, &AppImageUpdaterStandalone::handleCanceled);
    connect(m_Updater, &QAppImageUpdate::finished, this, &AppImageUpdaterStandalone::handleFinished);
    return;

}



void AppImageUpdaterStandalone::handleError(short errorCode)
{
    // Ignore all permission errors.
    if(errorCode == QAppImageUpdate::Error::NoReadPermission ||
            errorCode == QAppImageUpdate::Error::NoPermissionToReadSourceFile ||
            errorCode == QAppImageUpdate::Error::NoPermissionToReadWriteTargetFile) {
	    _pAuthorizationDialog.handleAuthorization(
		QAppImageUpdate::errorCodeToDescriptionString(errorCode),
		errorCode,
		m_AppImagePath);

	    return;
    }
    emit quit();
    return;
}

void AppImageUpdaterStandalone::handleFinished(QJsonObject info, short action)
{
    if(action != QAppImageUpdate::Action::UpdateWithGUIAndTorrent) {
	    return;
    }
    (void)info;
    emit quit();
    return;
}

void AppImageUpdaterStandalone::handleCanceled(short action)
{
    if(action != QAppImageUpdate::Action::UpdateWithGUIAndTorrent) {
  	    return;
    }
    emit quit();
    return;
}
