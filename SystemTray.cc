#include <QCoreApplication>

#include <SystemTray.hpp>

SystemTray::SystemTray(QObject *parent)
    : QObject(parent)

{
    auto arguments = QCoreApplication::arguments();
    if(arguments.size() != 1 && 
        arguments.at(1).toLower() == QString::fromUtf8("--minimized")) {
	    emit forceHide();
    }

    m_TIcon = new QSystemTrayIcon;
    m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/logo.png"))));
    
    auto menu = new QMenu; 
    menu->addAction(QString::fromUtf8("Show / Hide"), this, &SystemTray::showOrHide); 
    menu->addAction(QString::fromUtf8("Quit"), this, &SystemTray::quit);
    m_CurrentContextMenu = menu;

    m_TIcon->setContextMenu(menu);
    
    connect(m_TIcon, &QSystemTrayIcon::activated, this, &SystemTray::raise);
    m_TIcon->show();
}

SystemTray::~SystemTray()
{
	m_TIcon->deleteLater();
	m_CurrentContextMenu->deleteLater();
}

void SystemTray::raise() {
	emit raiseApp();
}

void SystemTray::notify(const QString &message) {
	m_TIcon->showMessage(QString::fromUtf8("AppImage Updater"), message);
}

void SystemTray::changeTrayIconToBlue() {
	m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/logo_blue.png"))));
}

void SystemTray::changeTrayIconToRed() {
	m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/logo_red.png"))));
}

void SystemTray::changeTrayIconDefault() {
	m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/logo.png"))));
}
