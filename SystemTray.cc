#include <SystemTray.hpp>

SystemTray::SystemTray(QObject *parent)
    : QObject(parent)

{
    m_TIcon = new QSystemTrayIcon;
    m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/logo.png"))));
    
    m_CurrentContextMenu = buildMenu();
    m_TIcon->setContextMenu(m_CurrentContextMenu);
    
    connect(m_TIcon, &QSystemTrayIcon::activated, this, &SystemTray::raise);
    m_TIcon->show();

 
}

SystemTray::~SystemTray()
{
	m_TIcon->deleteLater();
	if(m_CurrentContextMenu)
		delete m_CurrentContextMenu;
}

QMenu *SystemTray::buildMenu() {
    QMenu *menu = new QMenu;
    if(b_Hidden) {
    	menu->addAction(QString::fromUtf8("Show"),
		 	this,
			&SystemTray::show);
    } else {
	menu->addAction(QString::fromUtf8("Hide"),
		 	this,
			&SystemTray::hide); 
    }

    menu->addAction(QString::fromUtf8("Quit"),
		    this,
		    &SystemTray::quit);
    return menu;
}

void SystemTray::raise() {
	if(b_Hidden) {
		b_Hidden = false;
		emit isHiddenChanged();
		emit show();
	} else {
		emit raiseApp();
	}

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

void SystemTray::setIsHidden(bool value) { 
	b_Hidden = value;
	delete m_CurrentContextMenu;
	m_CurrentContextMenu = buildMenu();
	m_TIcon->setContextMenu(m_CurrentContextMenu);
	emit isHiddenChanged();
	return;
}

bool SystemTray::isHidden() const {
	return b_Hidden;
}	
