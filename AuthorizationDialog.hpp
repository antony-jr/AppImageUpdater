#ifndef AUTHORIZATION_DIALOG_HPP_INCLUDED
#define AUTHORIZATION_DIALOG_HPP_INCLUDED
#include <QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QJsonObject>
#include <ui_AuthorizationDialog.h>

class AuthorizationDialog : public QDialog
{
    Q_OBJECT
    QString program;
    QStringList arguments;
    Ui::AuthorizationDialog _pUi;
public:
    AuthorizationDialog(QWidget *parent = nullptr);
    ~AuthorizationDialog();
public Q_SLOTS:
    void handleAuthorization(QString, short, QString);
private Q_SLOTS:
    void showError(QString);
    void doAuthorize(QString, short, QString);
Q_SIGNALS:
    void started(void);
    void finished(QJsonObject);
};
#endif // AUTHORIZATION_DIALOG_HPP_INCLUDED
