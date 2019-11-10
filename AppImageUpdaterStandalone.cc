#include <AppImageUpdaterStandalone.hpp>
#include <QFileInfo>
#include <QScreen>

using AppImageUpdaterBridge::AppImageUpdaterDialog;
using AppImageUpdaterBridge::AppImageDeltaRevisioner;

AppImageUpdaterStandalone::AppImageUpdaterStandalone(QString AppImagePath, int flags, QObject *parent)
    : QObject(parent),
      flags(flags),
      m_AppImagePath(AppImagePath)
{

	m_Updater = new AppImageDeltaRevisioner(AppImagePath , this);
	m_Updater->setShowLog(true);
	QObject::connect(m_Updater,
		    &AppImageDeltaRevisioner::embededInformation, 
		    this, &AppImageUpdaterStandalone::handleAppImageInformation,
		    Qt::UniqueConnection);
	m_Updater->getAppImageEmbededInformation();
}

AppImageUpdaterStandalone::~AppImageUpdaterStandalone()
{
   if(_pUpdateDialog){
	_pUpdateDialog->hide();
	_pUpdateDialog->deleteLater();
   }
   return;
}

void AppImageUpdaterStandalone::handleAppImageInformation(QJsonObject info){
    QString applicationName;
    QJsonObject updInfo = (info["UpdateInformation"]).toObject();
    QString transportType = (updInfo["transport"]).toString();

    _pUpdateDialog = new AppImageUpdaterDialog(QPixmap(QString::fromUtf8(":/default_icon.png")), nullptr,flags);
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
    connect(_pUpdateDialog, &AppImageUpdaterDialog::canceled, this, &AppImageUpdaterStandalone::handleCanceled);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::error, this, &AppImageUpdaterStandalone::handleError);
    connect(_pUpdateDialog, &AppImageUpdaterDialog::finished, this, &AppImageUpdaterStandalone::handleFinished);
    return;

}



void AppImageUpdaterStandalone::handleError(QString eStr, short eCode)
{
    (void)eStr;
    (void)eCode;
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
    }
    emit quit();
    return;
}
