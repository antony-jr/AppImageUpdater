#include <AppImageUpdaterStandalone.hpp>
#include <QDesktopServices>
#include <QDesktopWidget>

using AppImageUpdaterBridge::AppImageUpdaterDialog;

AppImageUpdaterStandalone::AppImageUpdaterStandalone(const QString &AppImagePath, QObject *parent)
    : QObject(parent)
{
    if(!AppImagePath.isEmpty()) {
        _pUpdateDialog = new AppImageUpdaterDialog;
        _pUpdateDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        _pUpdateDialog->setMovePoint(QApplication::desktop()->screen(0)->rect().center() - _pUpdateDialog->rect().center());
        _pUpdateDialog->setShowBeforeStarted(true);
        _pUpdateDialog->setShowProgressDialog(true);
        _pUpdateDialog->setShowUpdateConfirmationDialog(false);
        _pUpdateDialog->setShowErrorDialog(true);
        _pUpdateDialog->setShowNoUpdateDialog(true);
        _pUpdateDialog->setShowFinishDialog(true);
        _pUpdateDialog->setIconPixmap(QPixmap(QString::fromUtf8(":/default_icon.png")));
        _pUpdateDialog->setShowLog(true);
        _pUpdateDialog->setAppImage(AppImagePath);
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
    _pUpdateDialog->hide();
    _pUpdateDialog->deleteLater();
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
    _pUpdateDialog->hide();
    emit quit();
    return;
}
