#ifndef SETTINGS_MANAGER_HPP_INCLUDED
#define SETTINGS_MANAGER_HPP_INCLUDED 
#include <QObject>
#include <QSettings>

class SettingsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isDarkMode READ isDarkMode WRITE setIsDarkMode NOTIFY isDarkModeChanged)
    Q_PROPERTY(bool isAllowSystemTrayNotification READ isASN WRITE setASN NOTIFY isASNChanged)
    Q_PROPERTY(bool isRunOnStartup READ isStartup WRITE setStartup NOTIFY startupChanged)
    Q_PROPERTY(bool isDecentralizedUpdateEnabled READ isBTEnabled WRITE setBTEnabled NOTIFY btEnabledChanged)
    Q_PROPERTY(bool isProxyEnabled READ isProxyEnabled WRITE setProxyEnabled NOTIFY proxyChanged)
    Q_PROPERTY(QString ProxyHost READ getProxyHost WRITE setProxyHost NOTIFY proxyHostChanged)
    Q_PROPERTY(int ProxyPort READ getProxyPort WRITE setProxyPort NOTIFY proxyPortChanged)
    Q_PROPERTY(QString ProxyType READ getProxyType WRITE setProxyType NOTIFY proxyTypeChanged)
    Q_PROPERTY(QString ProxyUser READ getProxyUser WRITE setProxyUser NOTIFY proxyUserChanged)
    Q_PROPERTY(QString ProxyPass READ getProxyPass WRITE setProxyPass NOTIFY proxyPassChanged) 
public:
    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager() override;
    	
    void setIsDarkMode(bool);
    bool isDarkMode() const;

    void setASN(bool);
    bool isASN() const;
  
    void setStartup(bool);
    bool isStartup() const;
 
    void setBTEnabled(bool);
    bool isBTEnabled() const;

    void setProxyEnabled(bool);
    bool isProxyEnabled() const;

    void setProxyHost(const QString&);
    void setProxyPort(int);
    void setProxyType(const QString&);
    void setProxyUser(const QString&);
    void setProxyPass(const QString&);
    
    QString getProxyHost() const;
    int getProxyPort() const;
    QString getProxyType() const;
    QString getProxyUser() const;
    QString getProxyPass() const;



signals:
    void isDarkModeChanged();
    void isASNChanged();
    void startupChanged();
    void btEnabledChanged();
    void proxyChanged();
    void proxyHostChanged();
    void proxyPortChanged();
    void proxyTypeChanged();
    void proxyUserChanged();
    void proxyPassChanged();
private:
    bool b_isDarkMode,
	 b_allowSystemNotifications,
	 b_RunOnStartup,
	 b_BTEnabled,
	 b_ProxyEnabled;
    QString m_ProxyHost,
	    m_ProxyType,
	    m_ProxyUser,
	    m_ProxyPass;
    int m_ProxyPort;
    QSettings m_Settings;
};
#endif /// SETTINGS_MANAGER_HPP_INCLUDED
