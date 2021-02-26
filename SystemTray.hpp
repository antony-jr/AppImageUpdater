#ifndef SYSTEM_TRAY_HPP_INCLUDED
#define SYSTEM_TRAY_HPP_INCLUDED 
#include <QObject>
#include <QString>
#include <QMenu>
#include <QSystemTrayIcon>

class SystemTray : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isHidden READ isHidden WRITE setIsHidden NOTIFY isHiddenChanged) 
public:
    explicit SystemTray(QObject *parent = nullptr);
    ~SystemTray() override;

public Q_SLOTS:
    void raise();
    void notify(const QString&);
    void changeTrayIconToBlue();
    void changeTrayIconToRed();
    void changeTrayIconDefault();

    void setIsHidden(bool);
    bool isHidden() const;
signals:
    void raiseApp();
    void show();
    void hide();
    void quit();
    void isHiddenChanged();
private:
    QMenu *buildMenu();
private:
    QMenu *m_CurrentContextMenu = nullptr;
    bool b_Hidden = false;
    QSystemTrayIcon *m_TIcon = nullptr;
};
#endif /// SYSTEM_TRAY_HPP_INCLUDED
