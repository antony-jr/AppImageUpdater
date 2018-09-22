#ifndef SETTINGS_DIALOG_HPP_INCLUDED
#define SETTINGS_DIALOG_HPP_INCLUDED
#include <QSettings>
#include <ui_SettingsDialog.h>

class SettingsDialog : public QDialog {
	Q_OBJECT
	Ui::AppImageUpdaterSettingsDialog _mUi;
	QSettings _mSettings;

	/*
	 * AutoStart Dynamic Desktop Entry.
	 * This desktop entry will be written in to
	 * $HOME/.config/autostart/AppImageUpdater.desktop.
	 * Thus the lastest instance will replace this file whenever
	 * the user checks the run on startup checkbox in the gui.
	*/

	QString _mDEntry = 
		QString::fromUtf8("[Desktop Entry]\nName=AppImageUpdater\nType=Application\nExec=%1 --minimized\nTerminal=false");
public:
	SettingsDialog(QWidget *parent = nullptr);
	~SettingsDialog();

	bool isShowUpdateDialogs() const;
	bool isShowSystemTrayNotificationMessages() const;
	bool isRunOnStartup() const;
private Q_SLOTS:
	void handleAccepted(void);
};
#endif // SETTINGS_HPP_INCLUDED
