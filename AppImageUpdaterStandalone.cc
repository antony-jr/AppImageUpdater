#include <AppImageUpdaterStandalone.hpp>
#include <QFileInfo>
#include <QScreen>

using AppImageUpdaterBridge::AppImageUpdaterDialog;
using AppImageUpdaterBridge::AppImageDeltaRevisioner;

AppImageUpdaterStandalone::AppImageUpdaterStandalone(QString AppImagePath, QObject *parent)
    : QObject(parent)
{

	m_Updater = new AppImageDeltaRevisioner(AppImagePath , this);
	m_Updater->setShowLog(true);

        int flags = (AppImageUpdaterDialog::Default ^ AppImageUpdaterDialog::ShowBeforeProgress) |
		    AppImageUpdaterDialog::AlertWhenAuthorizationIsRequired;
 	_pUpdateDialog = new AppImageUpdaterDialog(QPixmap(QString::fromUtf8(":/default_icon.png")),nullptr,flags);	

       	_pUpdateDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        _pUpdateDialog->move(QGuiApplication::primaryScreen()->geometry().center() - _pUpdateDialog->rect().center());
        _pUpdateDialog->init(m_Updater , QFileInfo(AppImagePath).baseName());

	/* Program logic. */
        connect(_pUpdateDialog, &AppImageUpdaterDialog::canceled, this, &AppImageUpdaterStandalone::handleCanceled);
        connect(_pUpdateDialog, &AppImageUpdaterDialog::error, this, &AppImageUpdaterStandalone::handleError);
        connect(_pUpdateDialog, &AppImageUpdaterDialog::finished, this, &AppImageUpdaterStandalone::handleFinished);
}

AppImageUpdaterStandalone::~AppImageUpdaterStandalone()
{
   if(_pUpdateDialog){
	_pUpdateDialog->hide();
	_pUpdateDialog->deleteLater();
   }
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
