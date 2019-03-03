#include <AppImageUpdaterStandalone.hpp>
#include <QDesktopServices>
#include <QDesktopWidget>

using AppImageUpdaterBridge::AppImageUpdaterDialog;

AppImageUpdaterStandalone::AppImageUpdaterStandalone(const QString &AppImagePath, QObject *parent)
    : QObject(parent)
{
    if(!AppImagePath.isEmpty()) {
        int flags = AppImageUpdaterDialog::AlertWhenAuthorizationIsRequired |
		    AppImageUpdaterDialog::ShowProgressDialog |
		    AppImageUpdaterDialog::ShowUpdateConfirmationDialog |
		    AppImageUpdaterDialog::ShowFinishedDialog |
		    AppImageUpdaterDialog::ShowErrorDialog | 
		    AppImageUpdaterDialog::NotifyWhenNoUpdateIsAvailable |
		    AppImageUpdaterDialog::ShowBeforeProgress; 
        
 	_pUpdateDialog = new AppImageUpdaterDialog(
		    		AppImagePath,
		    		QPixmap(QString::fromUtf8(":/default_icon.png")),
				(QWidget*)this,
				flags);
   
	_pUpdateDialog->setShowLog(true);	
       	_pUpdateDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        _pUpdateDialog->move(QApplication::desktop()->screen(0)->rect().center() - _pUpdateDialog->rect().center());
        _pUpdateDialog->setWindowIcon(QIcon(QPixmap(QString::fromUtf8(":/logo.png"))));
        _pUpdateDialog->init();

        /* Program logic. */
        connect(_pUpdateDialog, &AppImageUpdaterDialog::canceled, this, &AppImageUpdaterStandalone::handleCanceled);
        connect(_pUpdateDialog, &AppImageUpdaterDialog::error, this, &AppImageUpdaterStandalone::handleError);
        connect(_pUpdateDialog, &AppImageUpdaterDialog::finished, this, &AppImageUpdaterStandalone::handleFinished);
        return;
    }
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
