#include <QFile>
#include <QDir>
#include <QCoreApplication>

#include <SettingsManager.hpp>

static bool getBool(const QSettings &settings, const QString &key) {
	return settings.value(key).toBool();
}

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)

{
	m_Settings.sync();
	b_isDarkMode = getBool(m_Settings, "V2.isDarkMode");
	b_allowSystemNotifications = getBool(m_Settings, "V2.isAllowSystemNotifications");
	b_RunOnStartup = getBool(m_Settings, "V2.isRunOnStartup");
	b_BTEnabled = getBool(m_Settings, "V2.isDecentralizedUpdateEnabled");
	b_ProxyEnabled = getBool(m_Settings, "V2.isProxyEnabled");

	/// Get Proxy Settings
	m_ProxyHost = m_Settings.value("V2.ProxyHost").toString();
	m_ProxyPort = m_Settings.value("V2.ProxyPort").toInt();
	m_ProxyType = m_Settings.value("V2.ProxyType").toString();
	m_ProxyUser = m_Settings.value("V2.ProxyUser").toString();
	m_ProxyPass = m_Settings.value("V2.ProxyPass").toString();

	/// Setup Desktop Entry for Startup
  	auto arguments = QCoreApplication::arguments();
	m_DesktopEntry = QString::fromUtf8("[Desktop Entry]\n");
	m_DesktopEntry += QString::fromUtf8("Name=AppImageUpdater\n");
	m_DesktopEntry += QString::fromUtf8("Type=Application\n");
	m_DesktopEntry += QString::fromUtf8("Exec=%1 --minimized\n");
	m_DesktopEntry += QString::fromUtf8("Terminal=false\n");
	m_DesktopEntry = m_DesktopEntry.arg(
			 	QFileInfo(arguments.at(0)).absolutePath() +
                         	QString::fromUtf8("/") +
                            	QFileInfo(arguments.at(0)).fileName());
}

SettingsManager::~SettingsManager()
{
	m_Settings.sync();
}


void SettingsManager::setIsDarkMode(bool value) {
	b_isDarkMode = value;
	m_Settings.setValue("V2.isDarkMode", b_isDarkMode);
	emit isDarkModeChanged(); 
}

bool SettingsManager::isDarkMode() const {
	return b_isDarkMode;
}

void SettingsManager::setASN(bool value) {
	b_allowSystemNotifications = value;
	m_Settings.setValue("V2.isAllowSystemNotifications", b_allowSystemNotifications);
	emit isASNChanged();
}
bool SettingsManager::isASN() const {
	return b_allowSystemNotifications;
}

void SettingsManager::setStartup(bool value) {
	b_RunOnStartup = value;
	m_Settings.setValue("V2.isRunOnStartup", b_RunOnStartup);
	if(b_RunOnStartup) {
		QFile entryFile(
			QDir::homePath() + 
			QString::fromUtf8("/.config/autostart/AppImageUpdater.desktop"));
        		if(!entryFile.open(QIODevice::WriteOnly)) {
				b_RunOnStartup = false;
				m_Settings.setValue("V2.isRunOnStartup", false);
				return;
        		}
        		entryFile.write(m_DesktopEntry.toLatin1());
        		entryFile.close();
	} else {
		QFile::remove(
			QDir::homePath() +
			QString::fromUtf8("/.config/autostart/AppImageUpdater.desktop"));
	}	
	emit startupChanged(); 
}

bool SettingsManager::isStartup() const {
	return b_RunOnStartup;
}

void SettingsManager::setBTEnabled(bool value) {
	b_BTEnabled = value;
	m_Settings.setValue("V2.isDecentralizedUpdateEnabled", b_BTEnabled);	
	emit btEnabledChanged(); 
}
bool SettingsManager::isBTEnabled() const {
	return b_BTEnabled;
}

void SettingsManager::setProxyEnabled(bool value) {
	b_ProxyEnabled = value;
	m_Settings.setValue("V2.isProxyEnabled", b_ProxyEnabled);
	emit proxyChanged();	
}

bool SettingsManager::isProxyEnabled() const {
	return b_ProxyEnabled;
}

void SettingsManager::setProxyHost(const QString &value) {
	m_ProxyHost = value;
	m_Settings.setValue("V2.ProxyHost", m_ProxyHost);
	emit proxyHostChanged();
}
void SettingsManager::setProxyPort(int port) {
	m_ProxyPort = port;
	m_Settings.setValue("V2.ProxyPort", m_ProxyPort);
	emit proxyPortChanged();
}
void SettingsManager::setProxyType(const QString &value) {
	m_ProxyType = value;
	m_Settings.setValue("V2.ProxyType", value);
	emit proxyTypeChanged();
}
void SettingsManager::setProxyUser(const QString &value) {
	m_ProxyUser = value;
	m_Settings.setValue("V2.ProxyUser", value);
	emit proxyUserChanged();
}
void SettingsManager::setProxyPass(const QString &value) {
	m_ProxyPass = value;
	m_Settings.setValue("V2.ProxyPass", value);
	emit proxyPassChanged();
}
    
QString SettingsManager::getProxyHost() const {
	return m_ProxyHost;
}

int SettingsManager::getProxyPort() const {
	return m_ProxyPort;
}

QString SettingsManager::getProxyType() const {
	return m_ProxyType;
}

QString SettingsManager::getProxyUser() const {
	return m_ProxyUser;
}

QString SettingsManager::getProxyPass() const {
	return m_ProxyPass;
}
