#ifndef AUTHORIZATION_DIALOG_HPP_INCLUDED
#define AUTHORIZATION_DIALOG_HPP_INCLUDED
#include <QMessageBox>
#include <QDialog>
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
    void handleAuthorization(QString, QString);
private Q_SLOTS:
    void showError(QString);
Q_SIGNALS:
    void finished();
};
#endif // AUTHORIZATION_DIALOG_HPP_INCLUDED
