#include <QApplication>
#include <SingleApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QCommandLineParser>

#include "BuildConstants.hpp"
#include "SettingsManager.hpp"
#include "SystemTray.hpp"
#include "DropItemParser.hpp"
#include "Updater.hpp"
#include "Executer.hpp"

#include "global.hpp"
#include "AppImageImageProvider.hpp"

AppImageImageProvider *g_AppImageImageProvider = nullptr;

int main(int argc, char **argv)
{
    if(argc != 1 && !strcmp(argv[1], "--private")) {
	    QCoreApplication app(argc, argv);
	    return app.exec();
    }

    SingleApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    SingleApplication::setOrganizationName("antony-jr");
    SingleApplication::setApplicationName("AppImage Updater");

    qmlRegisterType<BuildConstants>("Core.BuildConstants", 1, 0, "BuildConstants");
    qmlRegisterType<SettingsManager>("Core.SettingsManager", 1, 0, "SettingsManager");
    qmlRegisterType<SystemTray>("Core.SystemTray", 1, 0, "SystemTray");
    qmlRegisterType<DropItemParser>("Core.DropItemParser", 1, 0, "DropItemParser");
    qmlRegisterType<Updater>("Core.Updater", 1, 0, "Updater");
    qmlRegisterType<Executer>("Core.Executer", 1, 0, "Executer");

    app.setWindowIcon(QIcon(QString::fromUtf8(":/logo.png")));
    QQuickStyle::setStyle("Material"); // Use Google Material Design
    QQmlApplicationEngine engine;
    
    g_AppImageImageProvider = new AppImageImageProvider;
    engine.addImageProvider("AIImage", g_AppImageImageProvider);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
