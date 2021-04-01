#ifndef SYSTEM_TRAY_HPP_INCLUDED
#define SYSTEM_TRAY_HPP_INCLUDED 
#include <QObject>
#include <QString>
#include <QMenu>
#include <QSystemTrayIcon>

class SystemTray : public QObject
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = nullptr);
    ~SystemTray() override;

public Q_SLOTS:
    void raise();
    void notify(const QString&);
    void changeTrayIconToBlue();
    void changeTrayIconToRed();
    void changeTrayIconDefault();
signals:
    void raiseApp();
    void showOrHide();
    void forceHide();
    void quit();
private:
    QMenu *m_CurrentContextMenu = nullptr;
    QSystemTrayIcon *m_TIcon = nullptr;
};
#endif /// SYSTEM_TRAY_HPP_INCLUDED
