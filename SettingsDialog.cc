#include <SettingsDialog.hpp>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QByteArray>

#define IS_CHECKED(checkbox) (checkbox->checkState() == Qt::Checked) ? true : false
#define BOOL_CHECKED(B) (B == true) ? Qt::Checked : Qt::Unchecked

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    _mUi.setupUi(this);
    (_mUi.doNotShowUpdateDialogs)->setCheckState(BOOL_CHECKED(_mSettings.value("doNotShowUpdateDialogs").toBool()));
    (_mUi.doNotShowSystemTrayMsgs)->setCheckState(BOOL_CHECKED(_mSettings.value("doNotShowSystemTrayMsgs").toBool()));
    (_mUi.runOnStartup)->setCheckState(BOOL_CHECKED(_mSettings.value("runOnStartup").toBool()));

    auto arguments = QCoreApplication::arguments();
    _mDEntry = _mDEntry.arg(QFileInfo(arguments.at(0)).absolutePath() +
                            QString::fromUtf8("/") +
                            QFileInfo(arguments.at(0)).fileName());
    connect(this, &QDialog::accepted, this, &SettingsDialog::handleAccepted);
    return;
}

SettingsDialog::~SettingsDialog()
{
    return;
}

bool SettingsDialog::isShowUpdateDialogs(void) const
{
    return !IS_CHECKED(_mUi.doNotShowUpdateDialogs);
}

bool SettingsDialog::isShowSystemTrayNotificationMessages(void) const
{
    return !IS_CHECKED(_mUi.doNotShowSystemTrayMsgs);
}

bool SettingsDialog::isRunOnStartup(void) const
{
    return IS_CHECKED(_mUi.runOnStartup);
}

void SettingsDialog::handleAccepted(void)
{
    _mSettings.setValue("doNotShowUpdateDialogs", IS_CHECKED(_mUi.doNotShowUpdateDialogs));
    _mSettings.setValue("doNotShowSystemTrayMsgs", IS_CHECKED(_mUi.doNotShowSystemTrayMsgs));

    if(IS_CHECKED(_mUi.runOnStartup)) {
        QFile entryFile(QDir::homePath() + QString::fromUtf8("/.config/autostart/AppImageUpdater.desktop"));
        if(!entryFile.open(QIODevice::WriteOnly)) {
            return;
        }
        entryFile.write(_mDEntry.toLatin1());
        entryFile.close();
        _mSettings.setValue("runOnStartup", true);
    } else {
        QFile::remove(QDir::homePath() + QString::fromUtf8("/.config/autostart/AppImageUpdater.desktop"));
        _mSettings.setValue("runOnStartup", false);
    }
    return;
}
